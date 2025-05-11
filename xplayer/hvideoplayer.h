#ifndef HVIDEOPLAYER_H
#define HVIDEOPLAYER_H

#include "hmedia.h"
class HVideoPlayer
{
public:
    HVideoPlayer();


public:
    void set_media(HMedia& media);
public:
    HMedia media;

};

#endif // HVIDEOPLAYER_H
