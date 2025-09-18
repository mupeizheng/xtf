#include "sonogramgenerator.h"
#include <QtMath>
#include <QPainter>

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

