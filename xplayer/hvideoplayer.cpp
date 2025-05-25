#include "hvideoplayer.h"

HVideoPlayer::HVideoPlayer() {
    width = 0;
    height = 0;
    duration = 0;
    start_time = 0;
    eof = 0;
    error = 0;
}

void HVideoPlayer::set_media(HMedia &media)
{
    this->media=media;
}
