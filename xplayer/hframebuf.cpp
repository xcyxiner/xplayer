#include "hframebuf.h"

HFrameBuf::HFrameBuf() {
    cache_num=DEFAUTL_FRAME_CACHENUM;
}

int HFrameBuf::push(HFrame *pFrame)
{
    std::lock_guard<std::mutex> locker(this->mutex);
    if(pFrame->isNull())
        return -10;


    if(frames.size()>=(size_t)cache_num){
        HFrame& frame=frames.front();
        frames.pop_front();
        free(frame.buf.len);
        if(frame.userdata){
            ::free(frame.userdata);
            frame.userdata=NULL;
        }
    }
    int ret=0;
    if(isNull()){
        resize(pFrame->buf.len*cache_num);
        ret=1;
    }

    HFrame frame;
    frame.buf.base=alloc(pFrame->buf.len);
    frame.buf.len=pFrame->buf.len;
    frame.copy(*pFrame);
    frames.push_back(frame);
    return ret;
}

int HFrameBuf::pop(HFrame *pFrame)
{
    std::lock_guard<std::mutex> locker(this->mutex);

    if(isNull())
        return -10;


    if(frames.size()==0){
        return -20;
    }

    HFrame frame = std::move(frames.front());
    frames.pop_front();

    if(frame.isNull()){
        return -30;
    }

    pFrame->copy(frame);
    return 0;
}

void HFrameBuf::clear()
{
    std::lock_guard<std::mutex> locker(this->mutex);
    frames.clear();
    HRingBuf::clear();
}
