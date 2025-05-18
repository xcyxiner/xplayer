#include "hvideowidget.h"
#include "hffplayer.h"
#include "ui_hvideowidget.h"

HVideoWidget::HVideoWidget(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::HVideoWidget)
{
    ui->setupUi(this);
}

HVideoWidget::~HVideoWidget()
{
    delete ui;
}

void HVideoWidget::open(HMedia &media)
{
    this->media=media;
    start();
}

void HVideoWidget::start()
{
    this->pImpl_player=new HFFPlayer();
    this->pImpl_player->set_media(this->media);
    this->pImpl_player->start();
}
