#ifndef SDL2AUDIOWND_H
#define SDL2AUDIOWND_H

#include "haudiownd.h"
#include <QWidget>
#include <mutex>
#include <queue>
#include "SDL.h"
#include "ffmpeg_util.h"
class SDL2AudioWnd : public HAudioWnd
{
    Q_OBJECT
public:
    explicit SDL2AudioWnd(QWidget *parent = nullptr);

signals:
private:
    SDL_AudioDeviceID m_audioDeviceID = 0;
    SDL_AudioSpec m_obtainedSpec;
    int m_sampleRate = 44100;
    int m_channels = 2;
    int m_currentOffset=0;
    SDL_AudioFormat m_format = AUDIO_S16;
    SwrContext* m_swrCtx = nullptr; // 声明重采样上下文变量
    AVChannelLayout out_ch_layout;   // 输出声道布局

    std::queue<QByteArray> m_audioQueue;
    std::mutex m_audioMutex;
    AVSampleFormat m_outSampleFmt;

public:
    bool initSDLAudio();
    void fillAudioBuffer(Uint8* stream, int len);
    void addAudioFrame(AVFrame *frame);
    void processAudioFrame(AVFrame* frame);
    AVSampleFormat convertSDLToFFmpegFormat(SDL_AudioFormat sdlFormat);
    // HAudioWnd interface
public:
    void playAudio() override;
};

#endif // SDL2AUDIOWND_H
