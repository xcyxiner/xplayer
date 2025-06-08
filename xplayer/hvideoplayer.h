#ifndef HVIDEOPLAYER_H
#define HVIDEOPLAYER_H

#include "hframebuf.h"
#include "hmedia.h"
#define DEFAULT_FPS         25
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
        this->audio_frame_buf.clear();
    }

    int push_frame(HFrame* pFrame){
        return frame_buf.push(pFrame);
    }

    int pop_frame(HFrame* pFrame){
        return frame_buf.pop(pFrame);
    }

    int audio_push_frame(HFrame* pFrame){
        return this->audio_frame_buf.push(pFrame);
    }

    int audio_pop_frame(HFrame* pFrame){
        return this->audio_frame_buf.pop(pFrame);
    }

public:
    HMedia media;

    int32_t width;
    int32_t height;
    int64_t duration;
    int64_t start_time;
    int eof;
    int error;

    int         fps;
protected:
    HFrameBuf frame_buf;//视频帧
    HFrameBuf audio_frame_buf;//音频帧
};

#endif // HVIDEOPLAYER_H
