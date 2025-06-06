#ifndef HFRAMEBUF_H
#define HFRAMEBUF_H

#include "hframe.h"
#include "hringbuf.h"
#include <deque>
#include <mutex>

#define DEFAUTL_FRAME_CACHENUM 10
class HFrameBuf : public HRingBuf
{
public:
    HFrameBuf();


    int push(HFrame* pFrame);
    int pop(HFrame* pFrame);
    void clear();

    int cache_num;
    std::deque<HFrame> frames;
    std::mutex mutex;
    std::condition_variable buffer_not_full;
    std::condition_variable buffer_not_empty;
};

#endif // HFRAMEBUF_H
