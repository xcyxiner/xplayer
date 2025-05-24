#ifndef GLWND_H
#define GLWND_H

#include "hglwidget.h"
#include "hvideownd.h"

#include <QWidget>

class GLWnd :  public HVideoWnd,HGLWidget
{
    Q_OBJECT
public:
    explicit GLWnd(QWidget *parent = nullptr);

signals:
};

#endif // GLWND_H
