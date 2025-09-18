#include "slantrangedialog.h"
#include "ui_slantrangedialog.h"

SlantRangeDialog::SlantRangeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SlantRangeDialog)
{
    ui->setupUi(this);
}

SlantRangeDialog::~SlantRangeDialog()
{
    delete ui;
}
