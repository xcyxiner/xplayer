#include "hframe.h"



bool HFrame::isNull()
{
    return w==0||h==0||buf.isNull();
}

void HFrame::copy(const HFrame &rhs)
{
    w=rhs.w;
    this->h=rhs.h;
    this->bpp=rhs.bpp;
    this->type=rhs.type;

    this->ts=rhs.ts;
    this->useridx=rhs.useridx;
    this->userdata=rhs.userdata;

    buf.copy(rhs.buf.base,rhs.buf.len);
}
