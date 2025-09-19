#include "slantrangedialog.h"
#include "ui_slantrangedialog.h"
#include "sonogramgenerator.h"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDebug>

SlantRangeDialog::SlantRangeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SlantRangeDialog)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // gamma slider
    ui->horizontalSlider->setRange(10, 300);  // gamma 0.1 - 3.0
    ui->horizontalSlider->setValue(100);
}

SlantRangeDialog::~SlantRangeDialog()
{
    delete ui;
}

void SlantRangeDialog::setData(const QVector<std::vector<uint8_t> > &port, const QVector<std::vector<uint8_t> > &starboard, const QImage &img)
{
    portDataAll = port;
    starboardDataAll = starboard;
    originalImage = img;
    currentImage = img;

    updateView();
}

void SlantRangeDialog::on_horizontalSlider_valueChanged(int value)
{
    double gamma = value / 100.0;
    currentImage = SonogramGenerator::applyGamma(originalImage, gamma);

    if (imageItem) {
        imageItem->setPixmap(QPixmap::fromImage(currentImage));
    }
}

void SlantRangeDialog::updateView()
{
    fitToWidth(ui->graphicsView, currentImage);
}

void SlantRangeDialog::fitToWidth(QGraphicsView *view, const QImage &image)
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

//更新初始化显示图像
void SlantRangeDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);   // 调用父类实现，保持默认行为
    updateView();                // 确保用正确的 viewport 宽度重新适配
}

