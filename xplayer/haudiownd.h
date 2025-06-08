#ifndef HAUDIOWND_H
#define HAUDIOWND_H

#include "hframe.h"
#include <QWidget>

class HAudioWnd : public QWidget
{
    Q_OBJECT
public:
    explicit HAudioWnd(QWidget *parent = nullptr);

signals:

public:
    HFrame last_frame;
};

#endif // HAUDIOWND_H
