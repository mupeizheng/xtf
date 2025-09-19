#ifndef XTFPARSE_H
#define XTFPARSE_H

#include <QObject>
#include "xtf.h"
#include <QVector>
#include <vector>

struct PingMeta {
    int numSamples;         // 样点数
    double timeDuration;    // 总采样时长 (s)
    double sampleInterval;  // 每个样点的采样间隔 (s)
    double soundVelocity;   // 声速 (m/s)，可能已经除过2
    double slantRange;      // 最大斜距 (m)
};

class xtfparse : public QObject
{
    Q_OBJECT
public:
    explicit xtfparse(QObject* parent = nullptr);
    ~xtfparse();

    // 解析 XTF 文件头和侧扫数据，返回左右舷数据
    void parseXtfHeader(const QString &filePath, QVector<std::vector<uint8_t>> &portData, QVector<std::vector<uint8_t>> &starboardData);

    PingMeta extractPingMeta(const XTFPINGHEADER& pingHeader, const XTFPINGCHANHEADER& chanHeader);

private:
    QVector<PingMeta> pingMetaList;   // 存很多 ping 的参数

};

#endif // XTFPARSE_H
