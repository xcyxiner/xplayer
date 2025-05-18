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
};

#endif // HFFPLAYER_H
