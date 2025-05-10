#ifndef HMULTIVIEW_H
#define HMULTIVIEW_H

#include <QWidget>
#include "hmedia.h"
namespace Ui {
class HMultiView;
}

class HMultiView : public QWidget
{
    Q_OBJECT

public:
    explicit HMultiView(QWidget *parent = nullptr);
    ~HMultiView();

private:
    Ui::HMultiView *ui;

public slots:
    void play(HMedia& media);

};

#endif // HMULTIVIEW_H
