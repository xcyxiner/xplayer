#ifndef HFFPLAYER_H
#define HFFPLAYER_H

#include "hframe.h"
#include "hthread.h"
#include "hvideoplayer.h"
#include "ffmpeg_util.h"
class HFFPlayer : public HVideoPlayer,public HThread
{
public:
    HFFPlayer();

    // HThread interface
public:
    int start() override;
    bool doPrepare() override;

private:

    int open();

private:
    AVFormatContext* fmt_ctx;
    AVDictionary* fmt_opts;

    AVCodecContext* codec_ctx;
    AVDictionary*   codec_opts;

    int video_stream_index;
    int audio_stream_index;
    int subtitle_stream_index;

    AVPixelFormat   src_pix_fmt;
    AVPixelFormat   dst_pix_fmt;

    SwsContext*     sws_ctx;

    AVPacket* packet;
    AVFrame* frame;

    uint8_t*        data[4];
    int             linesize[4];
    HFrame hframe;
};

#endif // HFFPLAYER_H
