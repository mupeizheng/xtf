#include <QFileDialog>
#include <QDebug>
#include <fstream>
#include <cmath>
#include "xtfparse.h"

xtfparse::xtfparse(QObject* parent)
    : QObject(parent)
{
}

xtfparse::~xtfparse()
{

}

void xtfparse::parseXtfHeader(const QString &filePath, QVector<std::vector<uint8_t> > &portData, QVector<std::vector<uint8_t> > &starboardData)
{
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) {
        qWarning() << "无法打开文件：" << filePath;
        return;
    }

    XTFFILEHEADER header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(XTFFILEHEADER));

    if (header.FileFormat != 0x7B) {
        qWarning() << "非标准 XTF 文件！";
        return;
    }

    qDebug() << "Header.NumberOfSonarChannels:" << header.NumberOfSonarChannels;

    if (header.NumberOfSonarChannels > 6) {
        int moveIndex = static_cast<int>(ceil((header.NumberOfSonarChannels - 6) / 8.0) * 1024);
        file.seekg(moveIndex, std::ios::cur);
    }

    portData.clear();
    starboardData.clear();

    while (!file.eof()) {
        XTFCHANHEADER chanHeader{};
        file.read(reinterpret_cast<char*>(&chanHeader), sizeof(XTFCHANHEADER));
        if (file.gcount() != sizeof(XTFCHANHEADER)) break;
        if (chanHeader.MagicNumber != 0xFACE) break;
        // qDebug()<<"XTFCHAHEADER size:"<<chanHeader.NumBytesThisRecord;
        switch (chanHeader.HeaderType) {
        case 0: { // 侧扫数据
            XTFPINGHEADER xtfpingHeader{};
            file.seekg(-static_cast<int>(sizeof(XTFCHANHEADER)), std::ios::cur);
            file.read(reinterpret_cast<char*>(&xtfpingHeader), sizeof(XTFPINGHEADER));
            qDebug()<<"SoundVelocity:"<< xtfpingHeader.SoundVelocity;
            for (int i = 0; i < header.NumberOfSonarChannels && i < 6; i++) {
                XTFPINGCHANHEADER xtfpingChanHeader{};
                file.read(reinterpret_cast<char*>(&xtfpingChanHeader), sizeof(XTFPINGCHANHEADER));
                // qDebug()<<"SlantRange:"<<xtfpingChanHeader.SlantRange;
                // qDebug()<<"GroundRange:"<<xtfpingChanHeader.GroundRange;
                // qDebug()<<"TimeDuration:"<<xtfpingChanHeader.TimeDuration;
                // qDebug()<<"SecondsPerPing:"<<xtfpingChanHeader.SecondsPerPing;

                qDebug()<<"NumSamples: "<<xtfpingChanHeader.NumSamples;

                //提取并保存每个 ping 的参数
                PingMeta meta = extractPingMeta(xtfpingHeader, xtfpingChanHeader);
                pingMetaList.append(meta);

                std::vector<uint8_t> rawData(xtfpingChanHeader.NumSamples, 0);

                if (header.ChanInfo[i].BytesPerSample == 1) {
                    file.read(reinterpret_cast<char*>(rawData.data()), xtfpingChanHeader.NumSamples);
                } else {
                    std::vector<int16_t> rawSamples(xtfpingChanHeader.NumSamples);
                    file.read(reinterpret_cast<char*>(rawSamples.data()), xtfpingChanHeader.NumSamples * sizeof(int16_t));
                    for (int k = 0; k < (int)xtfpingChanHeader.NumSamples; ++k) {
                        rawData[k] = 255 * rawSamples[k] / 32768;
                    }
                }

                if (i == 0) portData.append(rawData);       // 左舷
                else if (i == 1) starboardData.append(rawData); // 右舷
            }
            break;
        }
        default:
            file.seekg(chanHeader.NumBytesThisRecord, std::ios::cur);
        }
    }
}

PingMeta xtfparse::extractPingMeta(const XTFPINGHEADER &pingHeader, const XTFPINGCHANHEADER &chanHeader)
{
    PingMeta meta;

    meta.numSamples = chanHeader.NumSamples;
    meta.timeDuration = chanHeader.TimeDuration;

    if (meta.numSamples > 0)
        meta.sampleInterval = meta.timeDuration / meta.numSamples;
    else
        meta.sampleInterval = 0.0;

    // 声速：如果厂家已经存750就直接用，否则除以2
    double sv = pingHeader.SoundVelocity;
    if (sv > 1000) {
        sv = sv / 2.0;
    }
    meta.soundVelocity = sv;

    // 如果文件没提供 SlantRange，就自己算
    if (chanHeader.SlantRange > 0.0) {
        meta.slantRange = chanHeader.SlantRange;
    } else {
        double computed = (meta.numSamples - 1) * (meta.soundVelocity * meta.sampleInterval);
        int slantRangeInt = static_cast<int>(std::round(computed)); // 四舍五入
        meta.slantRange = slantRangeInt;
    }

    return meta;
}
