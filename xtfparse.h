#ifndef XTFPARSE_H
#define XTFPARSE_H

#include <QObject>
#include "xtf.h"
#include <QVector>
#include <vector>

class xtfparse : public QObject
{
    Q_OBJECT
public:
    explicit xtfparse(QObject* parent = nullptr);
    ~xtfparse();

    // 解析 XTF 文件头和侧扫数据，返回左右舷数据
    void parseXtfHeader(const QString &filePath, QVector<std::vector<uint8_t>> &portData, QVector<std::vector<uint8_t>> &starboardData);

};

#endif // XTFPARSE_H
