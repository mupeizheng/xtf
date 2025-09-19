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

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    void updateView();
    void fitToWidth(QGraphicsView *view, const QImage &image);

    void showEvent(QShowEvent *event) override;

};

#endif // SLANTRANGEDIALOG_H
