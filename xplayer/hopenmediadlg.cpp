#include "hopenmediadlg.h"
#include "ui_hopenmediadlg.h"
#include "QFileDialog"
HOpenMediaDlg::HOpenMediaDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HOpenMediaDlg)
{
    ui->setupUi(this);
}

HOpenMediaDlg::~HOpenMediaDlg()
{
    delete ui;
}

void HOpenMediaDlg::setCurrentIndex(int index)
{
    ui->tab->setCurrentIndex(index);
}

void HOpenMediaDlg::accept()
{
    switch (ui->tab->currentIndex()) {
    case MEDIA_TYPE_FILE:
        media.type=MEDIA_TYPE_FILE;
        media.src=ui->edit->text().toUtf8().data();
        break;
    default:
        break;
    }
    QDialog::accept();
}

void HOpenMediaDlg::on_buttonBox_accepted()
{
    this->accept();
}


void HOpenMediaDlg::on_pushButton_clicked()
{
    QString file=QFileDialog::getOpenFileName(this,tr("打开媒体文件"),"","视频文件(*.mp4)");
    if(!file.isEmpty()){
        ui->edit->setText(file);
    }
}

