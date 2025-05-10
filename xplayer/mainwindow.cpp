#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "avdef.h"
#include "hopenmediadlg.h"
#include "QMessageBox"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionopen_File_triggered()
{
    OpenMediaDlg(MEDIA_TYPE_FILE);
}

void MainWindow::OpenMediaDlg(int index)
{
    HOpenMediaDlg dlg(this);
    //后面存储index
    dlg.setCurrentIndex(index);
    if(dlg.exec()==QDialog::Accepted){
        // std::cout<<dlg.media.src<<std::endl;
        // QMessageBox msgBox;
        // msgBox.setText("dialog accepted");
        // msgBox.exec();

    }
}
