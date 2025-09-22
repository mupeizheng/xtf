#ifndef SONOGRAMGENERATOR_H
#define SONOGRAMGENERATOR_H

#include <QImage>
#include <QVector>
#include <vector>
#include <cstdint>

class SonogramGenerator
{
public:
    SonogramGenerator();

    // 输入左、右舷数据，生成声呐图像
    QImage createSonogram(const QVector<std::vector<uint8_t>>& portData,
                          const QVector<std::vector<uint8_t>>& starboardData,
                          bool combine = true);

    // 灰度/伽马矫正接口
    static QImage applyGamma(const QImage& src, double gamma);

    // 直方图均衡化接口
    static QImage applyHistogramEqualization(const QImage& src);

    //归一化接口
    static QImage applyNormalize(const QImage& src);

    // 强度拉伸（Stretch Intensity）
    static QImage applyStretchIntensity(const QImage& src);

    // 负片效果 (Negative)
    static QImage applyNegative(const QImage& src);


    // 斜距矫正并拼接左右舷
    static QImage applySlantRangeCorrection(const QVector<std::vector<uint8_t>> &portData, const QVector<std::vector<uint8_t>> &starboardData,const QVector<int> &portBottom,const QVector<int> &starboardBottom, double soundVelocity, double sampleInterval);


private:
    QImage vectorToImage(const QVector<std::vector<uint8_t>>& data);
};

#endif // SONOGRAMGENERATOR_H
