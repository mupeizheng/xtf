#ifndef SLANTRANGEDIALOG_H
#define SLANTRANGEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class SlantRangeDialog;
}

class SlantRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlantRangeDialog(QWidget *parent = nullptr);
    ~SlantRangeDialog();

    //获取原始数据图像
    void setData(const QVector<std::vector<uint8_t>> &port, const QVector<std::vector<uint8_t>> &starboard, const QImage &img);

private:
    Ui::SlantRangeDialog *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *imageItem = nullptr;

    QVector<std::vector<uint8_t>> portDataAll, starboardDataAll;
    QImage originalImage, currentImage;

    //水线
    QVector<int> portLine;
    QVector<int> starboardLine;

    bool slantCorrected = false; // 当前是否处于斜距矫正状态
    QImage correctedCache;           // 缓存的斜距矫正图

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_HistogramEqualizeBtn_clicked();
    void on_slantRangeCorrected_clicked();
    void on_StretchIntenistyBtn_clicked();
    void on_NegativeBtn_clicked();
    void on_RestoreBtn_clicked();

private:
    void updateView();
    void fitToWidth(QGraphicsView *view, const QImage &image);

    void showEvent(QShowEvent *event) override;

    void doBottomTrack();
    //寻找合适的开始位置
    int findAppropriateStartIdx(const std::vector<uint8_t>& samples, int startIdx);
    //移动平均平滑水线点
    QVector<int> smoothLine(const QVector<int>& line, int window = 50);

};

#endif // SLANTRANGEDIALOG_H
