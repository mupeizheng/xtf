#include "sonogramgenerator.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>

SonogramGenerator::SonogramGenerator()
{
}

QImage SonogramGenerator::createSonogram(const QVector<std::vector<uint8_t>>& portData,
                                         const QVector<std::vector<uint8_t>>& starboardData,
                                         bool combine)
{
    // 将左右舷分别转成 QImage
    QImage portImg = vectorToImage(portData);
    QImage starImg = vectorToImage(starboardData);

    if (!combine) {
        // 只返回左舷，调试用
        return portImg;
    }

    if (portImg.isNull() && starImg.isNull()) return QImage();

    int width = qMax(portImg.width(), starImg.width());
    int height = qMax(portImg.height(), starImg.height());

    // 创建一个大图，把左右拼接在一起
    QImage combined(width * 2, height, QImage::Format_Grayscale8);
    combined.fill(Qt::white);

    QPainter painter(&combined);
    painter.drawImage(0, 0, portImg);
    painter.drawImage(width, 0, starImg);
    painter.end();

    return combined;
}

QImage SonogramGenerator::vectorToImage(const QVector<std::vector<uint8_t>>& data)
{
    if (data.isEmpty()) return QImage();

    int height = data.size();
    int width  = data[0].size();

    QImage img(width, height, QImage::Format_Grayscale8);

    for (int y = 0; y < height; ++y) {
        const std::vector<uint8_t>& row = data[y];
        for (int x = 0; x < width; ++x) {
            uint8_t val = 255 - row[x]; //颜色反转
            img.setPixel(x, y, qRgb(val, val, val));
        }
    }

    // return img.mirrored(false, true); // Y 轴翻转，让ping顺序符合常见显示习惯
    return img;
}

QImage SonogramGenerator::applyGamma(const QImage& src, double gamma)
{
    if (src.isNull()) return QImage();

    QImage result = src.convertToFormat(QImage::Format_Grayscale8);

    uchar lut[256];
    double invGamma = 1.0 / gamma;
    for (int i = 0; i < 256; ++i) {
        lut[i] = static_cast<uchar>(qMin(255.0,
                                         qPow(i / 255.0, invGamma) * 255.0));
    }

    for (int y = 0; y < result.height(); ++y) {
        uchar* line = result.scanLine(y);
        for (int x = 0; x < result.width(); ++x) {
            line[x] = lut[line[x]];
        }
    }

    return result;
}

// 直方图均衡化实现
QImage SonogramGenerator::applyHistogramEqualization(const QImage& src)
{
    if (src.isNull()) return QImage();

    QImage result = src.convertToFormat(QImage::Format_Grayscale8);

    // 1. 统计直方图
    int hist[256] = {0};
    for (int y = 0; y < result.height(); ++y) {
        const uchar* line = result.constScanLine(y);
        for (int x = 0; x < result.width(); ++x) {
            hist[line[x]]++;
        }
    }

    // 2. 计算累积分布函数 (CDF)
    int totalPixels = result.width() * result.height();
    int cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i-1] + hist[i];
    }

    // 找到第一个非零的 cdf 值
    int cdfMin = 0;
    for (int i = 0; i < 256; ++i) {
        if (cdf[i] > 0) {
            cdfMin = cdf[i];
            break;
        }
    }

    // 3. 构建映射表
    uchar lut[256];
    for (int i = 0; i < 256; ++i) {
        lut[i] = static_cast<uchar>(
            qRound((cdf[i] - cdfMin) * 255.0 / (totalPixels - cdfMin))
            );
    }

    // 4. 应用映射表
    for (int y = 0; y < result.height(); ++y) {
        uchar* line = result.scanLine(y);
        for (int x = 0; x < result.width(); ++x) {
            line[x] = lut[line[x]];
        }
    }

    return result;
}

//归一化
// sonogramgenerator.cpp
QImage SonogramGenerator::applyNormalize(const QImage& src)
{
    if (src.isNull()) return QImage();

    QImage result = src.convertToFormat(QImage::Format_Grayscale8);

    int minVal = 255;
    int maxVal = 0;

    // 1. 找到图像的最小值和最大值
    for (int y = 0; y < result.height(); ++y) {
        const uchar* line = result.constScanLine(y);
        for (int x = 0; x < result.width(); ++x) {
            int val = line[x];
            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
    }

    if (maxVal == minVal) {
        // 图像所有像素相同，返回原图
        return result;
    }

    // 2. 归一化
    for (int y = 0; y < result.height(); ++y) {
        uchar* line = result.scanLine(y);
        for (int x = 0; x < result.width(); ++x) {
            line[x] = static_cast<uchar>(
                (line[x] - minVal) * 255.0 / (maxVal - minVal)
                );
        }
    }

    return result;
}

QImage SonogramGenerator::applyStretchIntensity(const QImage &src)
{
    if (src.isNull()) return src;

    int width = src.width();
    int height = src.height();

    int minVal = 255, maxVal = 0;

    // 找 min/max
    for (int y = 0; y < height; ++y) {
        const uchar *line = src.constScanLine(y);
        for (int x = 0; x < width; ++x) {
            int val = line[x];
            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
    }

    if (maxVal == minVal) return src;

    QImage dst(src.size(), QImage::Format_Grayscale8);

    for (int y = 0; y < height; ++y) {
        const uchar *srcLine = src.constScanLine(y);
        uchar *dstLine = dst.scanLine(y);
        for (int x = 0; x < width; ++x) {
            int val = srcLine[x];
            int stretched = (val - minVal) * 255 / (maxVal - minVal);
            dstLine[x] = static_cast<uchar>(qBound(0, stretched, 255));
        }
    }

    return dst;
}

QImage SonogramGenerator::applyNegative(const QImage &src)
{
    if (src.isNull()) return src;

    QImage dst(src.size(), QImage::Format_Grayscale8);

    for (int y = 0; y < src.height(); ++y) {
        const uchar *srcLine = src.constScanLine(y);
        uchar *dstLine = dst.scanLine(y);
        for (int x = 0; x < src.width(); ++x) {
            dstLine[x] = 255 - srcLine[x];  // 灰度反转
        }
    }

    return dst;
}

QImage SonogramGenerator::applySlantRangeCorrection(const QVector<std::vector<uint8_t> > &portData, const QVector<std::vector<uint8_t> > &starboardData, const QVector<int> &portBottom, const QVector<int> &starboardBottom, double soundVelocity, double sampleInterval)
{
    // if (portData.isEmpty() || starboardData.isEmpty()) {
    //     return QImage();
    // }

    // int numPings = qMin(portData.size(), starboardData.size());
    // int numSamples = portData[0].size();

    // // 单个采样点的单程距离分辨率 Δr
    // double deltaR = soundVelocity * sampleInterval;

    // // 计算最大斜距
    // double maxR = numSamples * deltaR;
    // int maxX = static_cast<int>(std::round(maxR));

    // // 输出图像（宽=左右舷合并，高=numPings）
    // QImage out(maxX * 2, numPings, QImage::Format_Grayscale8);
    // out.fill(0);

    // for (int i = 0; i < numPings; ++i) {
    //     int bottomP = (i < portBottom.size()) ? portBottom[i] : numSamples / 2;
    //     int bottomS = (i < starboardBottom.size()) ? starboardBottom[i] : numSamples / 2;

    //     double depthP = bottomP * deltaR;
    //     double depthS = bottomS * deltaR;

    //     // 左舷
    //     for (int j = bottomP; j < numSamples; ++j) {
    //         double R = j * deltaR;
    //         if (R < depthP) continue;
    //         double x = qSqrt(R * R - depthP * depthP);
    //         int xi = maxX - static_cast<int>(std::round(x)); // 左舷向左展开
    //         if (xi >= 0 && xi < maxX) {
    //             uint8_t val = 255 - portData[i][j]; // 颜色反转
    //             out.setPixel(xi, i, qRgb(portData[i][j], portData[i][j], portData[i][j]));
    //         }
    //     }

    //     // 右舷
    //     for (int j = bottomS; j < numSamples; ++j) {
    //         double R = j * deltaR;
    //         if (R < depthS) continue;
    //         double x = qSqrt(R * R - depthS * depthS);
    //         int xi = maxX + static_cast<int>(std::round(x));
    //         if (xi >= maxX && xi < 2 * maxX) {
    //             uint8_t val = 255 - starboardData[i][j]; // 颜色反转
    //             out.setPixel(xi, i, qRgb(starboardData[i][j], starboardData[i][j], starboardData[i][j]));
    //         }
    //     }
    // }

    // return out;

    if (portData.isEmpty() || starboardData.isEmpty()) {
        qDebug() << "No data loaded!";
        return QImage();
    }
    if (portBottom.size() != portData.size() ||
        starboardBottom.size() != starboardData.size()) {
        qDebug() << "Bottom line size mismatch!";
        return QImage();
    }
    int numPings = portData.size();

    int maxLeftWidth = portData[0].size();
    int maxRightWidth = starboardData[0].size();
    int totalWidth = maxLeftWidth + maxRightWidth;
    int totalHeight = numPings;

    QImage mergedImg(totalWidth, totalHeight, QImage::Format_RGB32);
    mergedImg.fill(Qt::white);

    // ---- 按 ping 处理 ----
    for (int ping = 0; ping < numPings; ++ping) {
        const auto& portRow = portData[ping];
        const auto& starRow = starboardData[ping];

        int startIdx = portBottom[ping];
        int endIdx   = starboardBottom[ping];
        if (startIdx < 0 || startIdx >= (int)portRow.size()) continue;
        if (endIdx <= 0 || endIdx >= (int)starRow.size()) continue;

        // 左舷有效区
        int leftWidth = startIdx + 1;
        QImage leftImg(leftWidth, 1, QImage::Format_RGB32);
        for (int x = 0; x < leftWidth; ++x) {
            int gray = 255 - portRow[x];
            leftImg.setPixel(x, 0, qRgb(gray, gray, gray));
        }
        // leftImg = leftImg.scaled(maxLeftWidth, 1); // 拉伸到统一宽度   最近邻插值->快速但不平滑
        leftImg = leftImg.scaled(maxLeftWidth, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //平滑插值


        // 右舷有效区
        int rightWidth = starRow.size() - endIdx;
        QImage rightImg(rightWidth, 1, QImage::Format_RGB32);
        for (int x = 0; x < rightWidth; ++x) {
            int gray = 255 - starRow[endIdx + x];
            rightImg.setPixel(x, 0, qRgb(gray, gray, gray));
        }
        // rightImg = rightImg.scaled(maxRightWidth, 1); // 拉伸到统一宽度
        rightImg = rightImg.scaled(maxRightWidth, 1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //平滑插值

        // 拼接到 mergedImg
        for (int x = 0; x < maxLeftWidth; ++x) {
            int g = qGray(leftImg.pixel(x, 0));
            mergedImg.setPixel(x, ping, qRgb(g, g, g));
        }
        for (int x = 0; x < maxRightWidth; ++x) {
            int g = qGray(rightImg.pixel(x, 0));
            mergedImg.setPixel(maxLeftWidth + x, ping, qRgb(g, g, g));
        }
    }

    return mergedImg;

}

