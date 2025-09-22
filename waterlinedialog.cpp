#include "waterlinedialog.h"
#include "ui_waterlinedialog.h"
#include "sonogramgenerator.h"   // 用到 gamma 矫正
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QShowEvent>
#include <QDebug>

WaterlineDialog::WaterlineDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WaterlineDialog)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    // 默认 gamma=1.0
    ui->horizontalSlider->setRange(10, 300); // 0.1 - 3.0
    ui->horizontalSlider->setValue(100);

    ui->portRadio->setChecked(true);   // 默认左舷
}

WaterlineDialog::~WaterlineDialog()
{
    delete ui;
}


void WaterlineDialog::setData(const QVector<std::vector<uint8_t> > &port, const QVector<std::vector<uint8_t> > &starboard, const QImage &img)
{
    portDataAll = port;
    starboardDataAll = starboard;
    originalImage = img;
    currentImage = img;

    updateView();
    doBottomTrack();
    doBottomTrackDisplay(true, false); // 默认绘制左舷
}

void WaterlineDialog::updateView()
{
    fitToWidth(ui->graphicsView, currentImage);
}

void WaterlineDialog::fitToWidth(QGraphicsView *view, const QImage &image)
{
    if (image.isNull()) return;

    if (!imageItem) {
        imageItem = scene->addPixmap(QPixmap::fromImage(image));
    } else {
        imageItem->setPixmap(QPixmap::fromImage(image));
    }

    qreal viewWidth = view->viewport()->width();
    qreal imgWidth  = image.width();
    if (imgWidth > 0) {
        qreal scaleFactor = viewWidth / imgWidth;
        QTransform transform;
        transform.scale(scaleFactor, scaleFactor);
        view->setTransform(transform);
    }

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scene->setSceneRect(imageItem->boundingRect());
}

void WaterlineDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);   // 调用父类实现，保持默认行为
    updateView();                // 确保用正确的 viewport 宽度重新适配
}

void WaterlineDialog::doBottomTrack()
{
    if (portDataAll.isEmpty() || starboardDataAll.isEmpty())
        return;

    // 删除之前的水线（避免重复绘制）
    for (auto item : bottomLineItems) {
        scene->removeItem(item);
        delete item;
    }
    bottomLineItems.clear();

    //记录上一ping有点采样点位置
    int prePortPing = -1;
    // int preStarboardPing = -1;
    int maxJump = 150;

    // ---- 左舷 ----
    for (int ping = 0 ; ping < portDataAll.size(); ++ping) {
        const std::vector<uint8_t>& samples = portDataAll[ping];
        int idx = -1;
        int CustomStartIdx = static_cast<int>(samples.size() * 0.7);
        int PortPingPos = findAppropriateStartIdx(samples, CustomStartIdx);

        for (int i = PortPingPos; i < (int)samples.size(); ++i) {
            if (samples[i] == 0) {
                int ZeroCount = 0;
                int checkRange = std::min(50, (int)samples.size() - i);
                for (int k = 0; k < checkRange; ++k) {
                    if (samples[i + k] < 1) ZeroCount++;
                }
                if (ZeroCount > checkRange*0.9) {
                    idx = i;
                    break;
                }
            }
        }

        if (ping == 0) {
            if (idx == -1) idx = CustomStartIdx;
            int bestmean = INT_MAX;
            for (int i = idx; i < (int)samples.size(); ++i) {
                if (samples[i] <= 3) {
                    int localsum = 0;
                    int localcount = 0;
                    int window = 100;
                    for (int k = 0; k < window; ++k) {
                        int Pos = i + k;
                        if (Pos < (int)samples.size()) {
                            localsum += samples[Pos];
                            localcount++;
                        } else break;
                    }
                    int localmean = localsum / std::max(1, localcount);

                    if (localmean <= 5 && localmean > 0) {
                        if (localmean <= bestmean) {
                            bestmean = localmean;
                            idx = i;
                        }
                    }
                }
            }
        } else {
            if (abs(idx - prePortPing) > maxJump || idx == -1) {
                int candidate = -1;
                int minPos = std::min(idx, prePortPing);
                minPos = minPos >= PortPingPos ? PortPingPos : minPos;
                int maxPos = std::max(idx, prePortPing);
                int bestMean = INT_MAX;

                for (int i = minPos; i <= maxPos; ++i) {
                    if (samples[i] <= 3) {
                        int localSum = 0;
                        int localCount = 0;
                        int win = 120;
                        for (int k = 0; k < win; ++k) {
                            int pos = i + k;
                            if (pos < (int)samples.size()) {
                                localSum += samples[pos];
                                localCount++;
                            }
                        }
                        int localMean = localSum / std::max(1, localCount);
                        if (localMean <= 5) {
                            if (localMean <= bestMean) {
                                bestMean = localMean;
                                candidate = i;
                            }
                        }
                    }
                }
                if (candidate != -1) idx = candidate;
            }
        }
        if (idx < 0) idx = samples.size() - 1;
        portBottomLine.append(idx);
        prePortPing = idx;
    }

    // ---- 右舷 ----
    for (int ping = 0; ping < starboardDataAll.size(); ++ping) {
        const std::vector<uint8_t>& samples = starboardDataAll[ping];
        int idx = -1;
        for (int i = 0; i < (int)samples.size() * 0.4; ++i) {
            if (samples[i] > 0) {
                int nonZeroCount = 0;
                int checkRange = std::min(50, (int)(samples.size()*0.4) - i);
                for (int k = 0; k < checkRange; ++k) {
                    if (samples[i + k] > 0) nonZeroCount++;
                }
                if (nonZeroCount > checkRange*0.9) {
                    idx = i;
                    break;
                }
            }
        }
        if (idx < 0) idx = 0;
        starboardBottomLine.append(idx);
    }

    // 平滑
    portsmoothLine = smoothLine(portBottomLine, 100);
    starboardsmoothLine = smoothLine(starboardBottomLine, 100);

    qDebug() << "底部追踪完成，已绘制曲线";
}

void WaterlineDialog::doBottomTrackDisplay(bool drawPort, bool drawStarboard)
{
    if (!scene) return;

    // 清理旧线（保留灰度图）
    for (auto item : bottomLineItems) {
        scene->removeItem(item);
        delete item;
    }
    bottomLineItems.clear();

    QPen pen(Qt::red, 3);
    int portWidth = portDataAll.isEmpty() ? 0 : portDataAll[0].size();

    if (drawPort && !portsmoothLine.isEmpty()) {
        QPainterPath path;
        path.moveTo(portsmoothLine[0], 0);
        for (int ping = 1; ping < portsmoothLine.size(); ++ping) {
            path.lineTo(portsmoothLine[ping], ping);
        }
        auto item = scene->addPath(path, pen);
        bottomLineItems.append(item);
    }

    if (drawStarboard && !starboardsmoothLine.isEmpty() && portWidth > 0) {
        QPainterPath path;
        path.moveTo(portWidth + starboardsmoothLine[0], 0);
        for (int ping = 1; ping < starboardsmoothLine.size(); ++ping) {
            path.lineTo(portWidth + starboardsmoothLine[ping], ping);
        }
        auto item = scene->addPath(path, pen);
        bottomLineItems.append(item);
    }
}

QVector<int> WaterlineDialog::smoothLine(const QVector<int> &line, int window)
{
    QVector<int> smoothed(line.size());
    for (int i = 0; i < line.size(); ++i) {
        int sum = 0, count = 0;
        for (int j = std::max(0, i-window); j <= std::min(i+window, line.size()-1); ++j) {
            sum += line[j];
            count++;
        }
        smoothed[i] = sum / count;
    }
    return smoothed;
}

int WaterlineDialog::findAppropriateStartIdx(const std::vector<uint8_t> &samples, int startIdx)
{
    if (samples.empty()) return -1;

    int idx = startIdx;
    if(startIdx == 0) return idx;
    // 如果当前位置强度值过小，则尝试往前寻找更合适的点
    if (samples[startIdx] == 0 || samples[startIdx-1] == 0) {
        int candidate = -1;

        // 1. 优先往前找第一个不为0的点
        for (int i = startIdx; i >= 0; --i) {
            if (samples[i] >= 3) {
                candidate = i+1;
                break;
            }
        }

        // 2. 如果没有严格为0的点，找一个接近0的点（阈值 >=5）
        if (candidate == -1) {
            for (int i = startIdx; i >= 0; --i) {
                if (samples[i] >= 5) {
                    candidate = i+1;
                    break;
                }
            }
        }

        if (candidate != -1) {
            idx = candidate;
        }
    }

    return idx;
}

// ---------- 按钮和 slider 的槽 ----------
void WaterlineDialog::on_horizontalSlider_valueChanged(int value)
{
    double gamma = value / 100.0;
    currentImage = SonogramGenerator::applyGamma(originalImage, gamma);

    if (imageItem) {
        imageItem->setPixmap(QPixmap::fromImage(currentImage));
    }
}

//左舷水线显示
void WaterlineDialog::on_portRadio_clicked()
{
    doBottomTrackDisplay(true, false);
}

//右舷水线显示
void WaterlineDialog::on_starboardRadio_clicked()
{
    doBottomTrackDisplay(false, true);
}

//直方图均衡化
void WaterlineDialog::on_HistoEqualize_clicked()
{
    currentImage = SonogramGenerator::applyHistogramEqualization(originalImage);

    if (imageItem) {
        imageItem->setPixmap(QPixmap::fromImage(currentImage));
    }
}

//归一化
void WaterlineDialog::on_NormalizeBtn_clicked()
{
    currentImage = SonogramGenerator::applyNormalize(originalImage);

    if (imageItem) {
        imageItem->setPixmap(QPixmap::fromImage(currentImage));
    }
}

