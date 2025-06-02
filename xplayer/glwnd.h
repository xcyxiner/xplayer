#ifndef GLWND_H
#define GLWND_H

#include "hglwidget.h"
#include "hvideownd.h"

#include <QWidget>

class GLWnd :  public HVideoWnd,HGLWidget
{
public:
    explicit GLWnd(QWidget *parent = nullptr);

signals:

    // HVideoWnd interface
public:
    virtual void setGeometry(const QRect &rc) override;
    virtual void update() override;

    // QOpenGLWidget interface
protected:
    virtual void paintGL() override;
};

#endif // GLWND_H
