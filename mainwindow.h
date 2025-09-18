#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "xtfparse.h"
#include "sonogramgenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fitToWidth(QGraphicsView* view, QImage& image);    //适配宽度显示


private:
    Ui::MainWindow *ui;

    xtfparse *xtfparser;   // 解析器对象
    QVector<std::vector<uint8_t>> portData;
    QVector<std::vector<uint8_t>> starboardData;

    SonogramGenerator generator;  // 声图生成器
    QGraphicsScene *scene;        // GraphicsScene，用来显示图像

private slots:
    void on_openFileButton_clicked();
    void on_bottomTrackButton_clicked();
    void on_Imagefusion_clicked();
};
#endif // MAINWINDOW_H
