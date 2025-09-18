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
            for (int i = 0; i < header.NumberOfSonarChannels && i < 6; i++) {
                XTFPINGCHANHEADER xtfpingChanHeader{};
                file.read(reinterpret_cast<char*>(&xtfpingChanHeader), sizeof(XTFPINGCHANHEADER));
                // qDebug()<<"SecondsPerPing:"<<xtfpingChanHeader.SecondsPerPing;
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
