#include "hffplayer.h"

HFFPlayer::HFFPlayer() {}

int HFFPlayer::start()
{
    HThread::start();
    return 0;
}

bool HFFPlayer::doPrepare()
{
    int ret=open();
    return ret;
}

int HFFPlayer::open()
{
    std::string ifile;
    AVInputFormat* ifmt=NULL;
    switch (media.type) {
    case MEDIA_TYPE_FILE:
        ifile=media.src;
        break;
    default:
        break;
        return -10;
    }
    int ret=0;

    fmt_ctx=avformat_alloc_context();
    if(fmt_ctx==NULL){
        ret=-10;
        return ret;
    }
}
