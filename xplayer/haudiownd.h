#ifndef HAUDIOWND_H
#define HAUDIOWND_H

#include "hframe.h"
#include <QWidget>
#include "ffmpeg_util.h"
class HAudioWnd : public QWidget
{
    Q_OBJECT
public:
    explicit HAudioWnd(QWidget *parent = nullptr);

signals:

public:
    virtual void playAudio();

public:
    AVFrame* last_frame;
};

#endif // HAUDIOWND_H
