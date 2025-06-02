#ifndef HFRAME_H
#define HFRAME_H

#include "hbuf.h"
#include <cstdint>
class HFrame
{
public:
    HBuf buf;
    int w;
    int h;
    int bpp;
    int type;

    uint64_t ts;
    int64_t useridx;
    void* userdata;

    HFrame(){
        w=h=bpp=type=0;
        ts=0;
        this->useridx=-1;
        this->userdata=NULL;
    }

    bool isNull();
    void copy(const HFrame& rhs);

};

#endif // HFRAME_H
