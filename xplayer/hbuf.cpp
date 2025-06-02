#include "hbuf.h"
#include <cstring>


size_t HBuf::getLen() const
{
    return len;
}

void HBuf::resize(size_t cap)
{
    if(cap==len)return;
    if(base==NULL){
        base=(char*)malloc(cap);
        memset(base,0,cap);
    }else{
        base=(char*)realloc(base,cap);
        if(cap>len){
            memset(base+len,0,cap-len);
        }
    }
    len=cap;
    cleanup_=true;
}

void HBuf::copy(void *data, size_t len)
{
    resize(len);
    memcmp(base,data,len);
}

bool HBuf::isNull()
{
    return base==NULL||len==0;
}

char *HBuf::getBase() const
{
    return base;
}
