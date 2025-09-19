#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waterlinedialog.h"
#include "slantrangedialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xtfparser = new xtfparse(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开 XTF 文件", "", "XTF Files (*.xtf)");
    if (fileName.isEmpty()) return;

    portData.clear();
    starboardData.clear();

    xtfparser->parseXtfHeader(fileName, portData, starboardData);

    if (portData.isEmpty() && starboardData.isEmpty()) {
        qWarning() << "没有读取到有效数据";
        return;
    }
    qDebug()<<"size:"<<portData[0].size();

    // 使用 SonogramGenerator 生成图像
    QImage sonarImg = generator.createSonogram(portData, starboardData, true);

    if (sonarImg.isNull()) {
        qWarning() << "生成声呐图失败";
        return;
    }

    fitToWidth(ui->graphicsView, sonarImg);
}

void MainWindow::on_bottomTrackButton_clicked()
{
    // 确保有图像
    QImage sonarImg = generator.createSonogram(portData, starboardData, true);
    if (sonarImg.isNull()) {
        qWarning() << "没有可用图像";
        return;
    }

    WaterlineDialog dlg(this);
    // dlg.setImage(sonarImg);
    dlg.setData(portData, starboardData, sonarImg);
    dlg.exec();
}

void MainWindow::on_Imagefusion_clicked()
{
    QImage sonarImg = generator.createSonogram(portData, starboardData, true);
    if (sonarImg.isNull()) {
        qWarning() << "没有可用图像";
        return;
    }

    SlantRangeDialog dlg(this);
    dlg.setData(portData, starboardData, sonarImg);
    dlg.exec();
}


void MainWindow::fitToWidth(QGraphicsView *view, QImage &image)
{
    if (image.isNull()) return;

    // 清空并重新设置 scene
    scene->clear();
    QGraphicsPixmapItem* item = scene->addPixmap(QPixmap::fromImage(image));
    view->setScene(scene);

    // 按宽度计算缩放比例
    qreal viewWidth = view->viewport()->width();
    qreal imgWidth  = image.width();
    if (imgWidth > 0) {
        qreal scaleFactor = viewWidth / imgWidth;

        QTransform transform;
        transform.scale(scaleFactor, scaleFactor);
        view->setTransform(transform);
    }

    // 水平方向关闭滚动条，垂直方向根据需要出现
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 保证 scene 大小与图像一致
    scene->setSceneRect(item->boundingRect());
}
