#include "sdl2audiownd.h"
#include <iostream>
#include <QtEndian>
SDL2AudioWnd::SDL2AudioWnd(QWidget *parent)
    : HAudioWnd(parent)
{
    this->initSDLAudio();
}
static void audioCallback(void *userdata, Uint8 *stream, int len)
{
    SDL2AudioWnd* audioWnd=static_cast<SDL2AudioWnd*>(userdata);
    audioWnd->fillAudioBuffer(stream,len);
}
bool SDL2AudioWnd::initSDLAudio()
{
    // 1. 初始化SDL2音频子系统
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        qCritical() << "SDL_Init error:" << SDL_GetError();
        return false;
    }

    // 2. 设置期望的音频参数
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);

    // 使用音频帧中的参数（这些应该在解码时设置）
    desiredSpec.freq = m_sampleRate;      // 采样率 (例如44100)
    desiredSpec.channels = m_channels;    // 声道数 (例如2)
    desiredSpec.format = m_format;        // 样本格式 (例如AUDIO_S16)
    desiredSpec.samples = 1024;           // 音频缓冲区大小(建议2048或4096)
    desiredSpec.callback = audioCallback; // 回调函数
    desiredSpec.userdata = this;          // 传递实例指针

    // 3. 打开音频设备
    m_audioDeviceID = SDL_OpenAudioDevice(
        nullptr,      // 使用默认设备
        0,            // 0=输出设备,1=输入设备
        &desiredSpec,
        &m_obtainedSpec,
        SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
            SDL_AUDIO_ALLOW_CHANNELS_CHANGE
        );

    if (m_audioDeviceID == 0) {
        qCritical() << "Failed to open audio device:" << SDL_GetError();
        return false;
    }

    // 4. 检查实际获得的音频参数
    if (desiredSpec.format != m_obtainedSpec.format) {
        qWarning() << "Using different audio format:"
                   << "Desired:" << SDL_GetPixelFormatName(desiredSpec.format)
                   << "Obtained:" << SDL_GetPixelFormatName(m_obtainedSpec.format);
    }

    if (desiredSpec.channels != m_obtainedSpec.channels) {
        qWarning() << "Using different channel count:"
                   << "Desired:" << (int)desiredSpec.channels
                   << "Obtained:" << (int)m_obtainedSpec.channels;
    }

    if (desiredSpec.freq != m_obtainedSpec.freq) {
        qWarning() << "Using different sample rate:"
                   << "Desired:" << desiredSpec.freq
                   << "Obtained:" << m_obtainedSpec.freq;
    }

    // 5. 保存实际获得的参数
    m_sampleRate = m_obtainedSpec.freq;
    m_channels = m_obtainedSpec.channels;
    m_format = m_obtainedSpec.format;

    m_outSampleFmt = convertSDLToFFmpegFormat(m_format);

    // 6. 开始播放
    SDL_PauseAudioDevice(m_audioDeviceID, 0);

    qDebug() << "SDL2 audio initialized:"
             << "Format:" << m_format
             << "Sample rate:" << m_sampleRate
             << "Channels:" << m_channels
             << "Buffer size:" << m_obtainedSpec.samples;

    return true;
}
AVSampleFormat SDL2AudioWnd::convertSDLToFFmpegFormat(SDL_AudioFormat sdlFormat) {
    switch (sdlFormat) {
    case AUDIO_U8:     return AV_SAMPLE_FMT_U8;
    case AUDIO_S16:    return AV_SAMPLE_FMT_S16;
    case AUDIO_S32:    return AV_SAMPLE_FMT_S32;
    case AUDIO_F32:    return AV_SAMPLE_FMT_FLT;
    default:           return AV_SAMPLE_FMT_NONE;
    }
}

void SDL2AudioWnd::fillAudioBuffer(Uint8 *stream, int len)
{
    // 清空目标缓冲区（填充静音）
    SDL_memset(stream, 0, len);

    // 锁定音频队列
    std::unique_lock<std::mutex> lock(m_audioMutex, std::try_to_lock);
    if (!lock.owns_lock() || m_audioQueue.empty()) {
        return; // 无法锁定或队列为空
    }

    int bytesWritten = 0;

    // 持续填充直到缓冲区满或队列空
    while (bytesWritten < len && !m_audioQueue.empty()) {
        QByteArray& audioData = m_audioQueue.front();
        int remaining = audioData.size() - m_currentOffset;

        // 计算本次可写入的字节数
        int toCopy = std::min(len - bytesWritten, remaining);

        // 复制数据到音频缓冲区
        SDL_memcpy(stream + bytesWritten,
                   audioData.constData() + m_currentOffset,
                   toCopy);

        // 更新指针和计数器
        bytesWritten += toCopy;
        m_currentOffset += toCopy;

        // 检查当前数据块是否用完
        if (m_currentOffset >= audioData.size()) {
            // 移除已用完的数据块
            m_audioQueue.pop();
            m_currentOffset = 0;
        }
    }
}

void SDL2AudioWnd::addAudioFrame(AVFrame *frame)
{
    // 确保帧有效
    if (!frame || !frame->data[0] || frame->nb_samples <= 0) {
        return;
    }
    // 计算PCM数据大小
    int bytesPerSample = SDL_AUDIO_BITSIZE(m_format) / 8;
    int dataSize = frame->nb_samples * m_channels * bytesPerSample;

    // 转换为QByteArray
    QByteArray audioData(reinterpret_cast<const char*>(frame->data[0]), dataSize);

    // 添加到队列（线程安全）
    std::lock_guard<std::mutex> lock(m_audioMutex);
    m_audioQueue.push(audioData);

    // 限制队列大小（防止内存无限增长）
    const int maxQueueSize = 10; // 最多存储10个数据块
    while (m_audioQueue.size() > maxQueueSize) {
        m_audioQueue.pop();
        qWarning() << "Audio buffer overflow, dropped frame";
    }
}

void SDL2AudioWnd::processAudioFrame(AVFrame *inputFrame)
{

    AVChannelLayout in_ch_layout;
    av_channel_layout_copy(&in_ch_layout, &inputFrame->ch_layout);

    // 2. 设置输出声道布局（立体声）
    AVChannelLayout out_ch_layout;
    av_channel_layout_default(&out_ch_layout, m_channels);  // 2表示立体声


    // 2. 检查是否需要重采样
    bool needsResample =
        (inputFrame->sample_rate != this->m_sampleRate) ||
        !av_channel_layout_compare(&in_ch_layout, &out_ch_layout) ||
        (inputFrame->format != m_outSampleFmt);

    // 3. 创建或更新重采样上下文（如果需要）
    if (!m_swrCtx && needsResample) {
        m_swrCtx = swr_alloc();

        // 设置重采样参数（兼容FFmpeg 5.0+）
        av_opt_set_chlayout(m_swrCtx, "in_chlayout", &in_ch_layout, 0);
        av_opt_set_chlayout(m_swrCtx, "out_chlayout", &out_ch_layout, 0);
        av_opt_set_int(m_swrCtx, "in_sample_rate", inputFrame->sample_rate, 0);
        av_opt_set_int(m_swrCtx, "out_sample_rate", m_sampleRate, 0);
        av_opt_set_sample_fmt(m_swrCtx, "in_sample_fmt", static_cast<AVSampleFormat>(inputFrame->format), 0);
        av_opt_set_sample_fmt(m_swrCtx, "out_sample_fmt",m_outSampleFmt, 0);

        int ret=swr_init(m_swrCtx);
        // 初始化重采样器
        if (ret < 0) {
            qWarning() << "Failed to initialize resampler";
            swr_free(&m_swrCtx);
            return;
        }
    }

    // 4. 创建输出帧
    AVFrame* outputFrame = av_frame_alloc();
    outputFrame->sample_rate = this->m_sampleRate;
    outputFrame->format = m_outSampleFmt;

    // 设置输出声道布局
    av_channel_layout_copy(&outputFrame->ch_layout, &out_ch_layout);

    // 5. 计算输出样本数
    int64_t delay = swr_get_delay(m_swrCtx, inputFrame->sample_rate);
    int64_t out_samples = av_rescale_rnd(
        delay + inputFrame->nb_samples,
       m_sampleRate,
        inputFrame->sample_rate,
        AV_ROUND_UP
        );
    outputFrame->nb_samples = (int)out_samples;

    // 6. 分配输出帧缓冲区
    int ret = av_frame_get_buffer(outputFrame, 0);
    if (ret < 0) {
        qWarning() << "Failed to allocate output frame buffer";
        av_frame_free(&outputFrame);
        return;
    }

    // 7. 执行重采样
    ret = swr_convert_frame(m_swrCtx, outputFrame, inputFrame);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning() << "Resampling failed:" << errbuf;
        av_frame_free(&outputFrame);
        return;
    }

    addAudioFrame(outputFrame);
}


void SDL2AudioWnd::playAudio()
{
    this->processAudioFrame(this->last_frame);
 // this->addAudioFrame(this->last_frame);
}


