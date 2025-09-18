#ifndef SLANTRANGEDIALOG_H
#define SLANTRANGEDIALOG_H

#include <QDialog>

namespace Ui {
class SlantRangeDialog;
}

class SlantRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlantRangeDialog(QWidget *parent = nullptr);
    ~SlantRangeDialog();

private:
    Ui::SlantRangeDialog *ui;
};

#endif // SLANTRANGEDIALOG_H
