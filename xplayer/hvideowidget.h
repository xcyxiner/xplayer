#ifndef HVIDEOWIDGET_H
#define HVIDEOWIDGET_H

#include "haudiownd.h"
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
    HAudioWnd *audiownd;

public:
    aspect_ratio_t aspect_ratio;
 int     fps;
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
