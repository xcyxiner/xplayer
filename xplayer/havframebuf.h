#ifndef HAVFRAMEBUF_H
#define HAVFRAMEBUF_H

#include "hringbuf.h"
#include <condition_variable>
#include <deque>
#include "ffmpeg_util.h"
#define DEFAUTL_FRAME_CACHENUM 10
class HAVFrameBuf
{
public:
    HAVFrameBuf();

    int push(AVFrame* pFrame);
    int pop(AVFrame* pFrame);
    void clear();

    int cache_num;
    std::deque<AVFrame*> frames;
    std::mutex mutex;
    std::condition_variable buffer_not_full;
    std::condition_variable buffer_not_empty;
};

#endif // HAVFRAMEBUF_H
