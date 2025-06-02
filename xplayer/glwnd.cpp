#include "glwnd.h"

GLWnd::GLWnd(QWidget *parent)
    :  HVideoWnd(parent),HGLWidget(parent)
{}

void GLWnd::setGeometry(const QRect &rc)
{
    HGLWidget::setGeometry(rc);
}

void GLWnd::update()
{
    HGLWidget::update();
}

void GLWnd::paintGL()
{
    HGLWidget::paintGL();

    if(this->last_frame.isNull()){

    }else{
        drawFrame(&this->last_frame);
    }
}
