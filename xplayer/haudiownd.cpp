#include "haudiownd.h"

HAudioWnd::HAudioWnd(QWidget *parent)
    : QWidget{parent}
{
     this->last_frame = av_frame_alloc();
}

void HAudioWnd::playAudio()
{

}
