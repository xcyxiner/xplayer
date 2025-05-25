#include "hvideowidget.h"
#include "glwnd.h"
#include "hffplayer.h"
#include "ui_hvideowidget.h"
#include <QTimer>
HVideoWidget::HVideoWidget(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::HVideoWidget)
{
    ui->setupUi(this);
    this->videownd=new GLWnd(this);
    this->aspect_ratio.type=ASPECT_FULL;

    this->timer=new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerUpdate()));
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
    int ret= this->pImpl_player->start();
    if(ret!=0){

    }else{
        //
        onOpenSucceed();
    }
}

void HVideoWidget::onOpenSucceed()
{
    setAspectRatio(aspect_ratio);
}

void HVideoWidget::setAspectRatio(aspect_ratio_t aspect_ratio)
{
    this->aspect_ratio = aspect_ratio;
    int border = 1;
    int scr_w = width() - border * 2;
    int scr_h = height() - border * 2;
    if (scr_w <= 0 || scr_h <= 0) return;
    int pic_w = 0;
    int pic_h = 0;
    if (pImpl_player) {
        pic_w = pImpl_player->width;
        pic_h = pImpl_player->height;
    }
    if (pic_w == 0) pic_w = scr_w;
    if (pic_h == 0) pic_h = scr_h;
    // calc videownd rect
    int dst_w, dst_h;
    switch (aspect_ratio.type) {
    case ASPECT_FULL:
        dst_w = scr_w;
        dst_h = scr_h;
        break;
    case ASPECT_PERCENT:
    case ASPECT_ORIGINAL_SIZE:
    case ASPECT_CUSTOM_SIZE:
    case ASPECT_ORIGINAL_RATIO:
    case ASPECT_CUSTOM_RATIO:
        break;
    }
    dst_w = std::min(dst_w, scr_w);
    dst_h = std::min(dst_h, scr_h);
    // align 4
    dst_w = dst_w >> 2 << 2;
    dst_h = dst_h >> 2 << 2;

    int x = border + (scr_w - dst_w) / 2;
    int y = border + (scr_h - dst_h) / 2;
    videownd->setGeometry(QRect(x, y, dst_w, dst_h));
}

void HVideoWidget::onTimerUpdate()
{
    if(this->pImpl_player==NULL)return;
    //提取一帧

    // update video frame
    videownd->update();
}
