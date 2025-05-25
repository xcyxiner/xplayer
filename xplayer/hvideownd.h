#ifndef HVIDEOWND_H
#define HVIDEOWND_H

#include "hframe.h"
#include <QWidget>

class HVideoWnd : public QWidget
{
    Q_OBJECT
public:
    explicit HVideoWnd(QWidget *parent = nullptr);

    virtual void setGeometry(const QRect& rc) = 0;
    virtual void update() = 0;
signals:

public:
    HFrame last_frame;
};

#endif // HVIDEOWND_H
