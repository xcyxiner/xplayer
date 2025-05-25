#ifndef HVIDEOWIDGET_H
#define HVIDEOWIDGET_H

#include "hmedia.h"
#include "hvideoplayer.h"
#include "hvideownd.h"
#include <QFrame>

namespace Ui {
class HVideoWidget;
}

class HVideoWidget : public QFrame
{
    Q_OBJECT

public:
    explicit HVideoWidget(QWidget *parent = nullptr);
    ~HVideoWidget();

private:
    Ui::HVideoWidget *ui;
    HVideoWnd *videownd;

public:
    aspect_ratio_t aspect_ratio;

public slots:
    void open(HMedia& media);
    void start();

    void onOpenSucceed();

    void setAspectRatio(aspect_ratio_t aspect_ratio);

     void onTimerUpdate();

private:
    HMedia media;
    HVideoPlayer* pImpl_player;

    QTimer*         timer;
};

#endif // HVIDEOWIDGET_H
