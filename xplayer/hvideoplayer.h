#ifndef HVIDEOPLAYER_H
#define HVIDEOPLAYER_H

#include "hframebuf.h"
#include "hmedia.h"
class HVideoPlayer
{
public:
    HVideoPlayer();


public:
    void set_media(HMedia& media);

    virtual int start()=0;
    virtual int stop()=0;
    virtual int pause()=0;
    virtual int resume()=0;

    void clear_frame_cache(){
        this->frame_buf.clear();
    }

    int push_frame(HFrame* pFrame){
        return frame_buf.push(pFrame);
    }

    int pop_frame(HFrame* pFrame){
        return frame_buf.pop(pFrame);
    }

public:
    HMedia media;

    int32_t width;
    int32_t height;
    int64_t duration;
    int64_t start_time;
    int eof;
    int error;

protected:
    HFrameBuf frame_buf;
};

#endif // HVIDEOPLAYER_H
