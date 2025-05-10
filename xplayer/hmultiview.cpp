#include "hmultiview.h"
#include "ui_hmultiview.h"

HMultiView::HMultiView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HMultiView)
{
    ui->setupUi(this);
}

HMultiView::~HMultiView()
{
    delete ui;
}

void HMultiView::play(HMedia &media)
{

}
