#ifndef HOPENMEDIADLG_H
#define HOPENMEDIADLG_H

#include "hmedia.h"
#include <QDialog>

namespace Ui {
class HOpenMediaDlg;
}

class HOpenMediaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HOpenMediaDlg(QWidget *parent = nullptr);
    ~HOpenMediaDlg();

private:
    Ui::HOpenMediaDlg *ui;

public:
 void setCurrentIndex(int index);

public slots:
    virtual void accept();

public:
    HMedia media;
private slots:
    void on_pushButton_clicked();
};

#endif // HOPENMEDIADLG_H
