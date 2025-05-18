#ifndef HFFPLAYER_H
#define HFFPLAYER_H

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


};

#endif // HFFPLAYER_H
