#ifndef HVIDEOWIDGET_H
#define HVIDEOWIDGET_H

#include "hmedia.h"
#include "hvideoplayer.h"
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

public slots:
    void open(HMedia& media);
    void start();

private:
    HMedia media;
    HVideoPlayer* pImpl_player;
};

#endif // HVIDEOWIDGET_H
