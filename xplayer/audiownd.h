#ifndef AUDIOWND_H
#define AUDIOWND_H

#include "haudiownd.h"

#include <QWidget>
class AudioWnd : public HAudioWnd
{
    Q_OBJECT
public:
    explicit AudioWnd(QWidget *parent = nullptr);

signals:

public:


    // HAudioWnd interface
public:
    virtual void playAudio() override;
};

#endif // AUDIOWND_H
