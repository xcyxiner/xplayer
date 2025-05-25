#ifndef HVIDEOPLAYER_H
#define HVIDEOPLAYER_H

#include "hmedia.h"
class HVideoPlayer
{
public:
    HVideoPlayer();


public:
    void set_media(HMedia& media);

    virtual int start()=0;
public:
    HMedia media;

    int32_t width;
    int32_t height;
    int64_t duration;
    int64_t start_time;
    int eof;
    int error;
};

#endif // HVIDEOPLAYER_H
