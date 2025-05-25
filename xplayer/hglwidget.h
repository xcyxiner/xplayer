#ifndef HGLWIDGET_H
#define HGLWIDGET_H


#include "QtOpenGLWidgets/qopenglwidget.h"
class HGLWidget : public QOpenGLWidget
{
public:
    explicit HGLWidget(QWidget *parent = nullptr);

signals:

    // QOpenGLWidget interface
protected:


    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};

#endif // HGLWIDGET_H
