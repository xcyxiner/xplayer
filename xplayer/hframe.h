#ifndef HFRAME_H
#define HFRAME_H

#include "hbuf.h"
class HFrame
{
public:
    HBuf buf;
    int w;
    int h;
    int bpp;
    int type;


    HFrame(){
        w=h=bpp=type=0;
    }
};

#endif // HFRAME_H
