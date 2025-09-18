#ifndef WATERLINEDIALOG_H
#define WATERLINEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QImage>

namespace Ui {
class WaterlineDialog;
}

class WaterlineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaterlineDialog(QWidget *parent = nullptr);
    ~WaterlineDialog();

    // 设置图像接口
    // void setImage(const QImage &img);

    void setData(const QVector<std::vector<uint8_t>>& port, const QVector<std::vector<uint8_t>>& starboard, const QImage& img);

private:
    Ui::WaterlineDialog *ui;

    QGraphicsScene *scene;
    QImage originalImage;   // 原始图像
    QImage currentImage;    // 当前显示的图像

    void updateView();
    void fitToWidth(QGraphicsView* view, const QImage& image);
    void showEvent(QShowEvent *event) override;

    // 底部追踪相关
    QVector<std::vector<uint8_t>> portDataAll;
    QVector<std::vector<uint8_t>> starboardDataAll;
    QVector<int> portBottomLine;
    QVector<int> starboardBottomLine;
    QVector<int> portsmoothLine;
    QVector<int> starboardsmoothLine;
    QList<QGraphicsItem*> bottomLineItems;

    void doBottomTrack();
    void doBottomTrackDisplay(bool drawPort, bool drawStarboard);
    //移动平均平滑水线点
    QVector<int> smoothLine(const QVector<int>& line, int window = 50);
    //寻找合适的开始位置
    int findAppropriateStartIdx(const std::vector<uint8_t>& samples, int startIdx);

    QGraphicsPixmapItem* imageItem = nullptr;  // 灰度图

private slots:
    void on_horizontalSlider_valueChanged(int value); //gamma矫正

    void on_portRadio_clicked();
    void on_starboardRadio_clicked();

    void on_HistoEqualize_clicked();

    void on_NormalizeBtn_clicked();
};

#endif // WATERLINEDIALOG_H
