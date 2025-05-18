#include "hffplayer.h"

HFFPlayer::HFFPlayer() {

    fmt_opts=NULL;
    fmt_ctx=NULL;
    codec_ctx = NULL;
    codec_opts=NULL;
}

int HFFPlayer::start()
{
    HThread::start();
    return 0;
}

bool HFFPlayer::doPrepare()
{
    int ret=open();
    return ret;
}

int HFFPlayer::open()
{
    std::string ifile;
    AVInputFormat* ifmt=NULL;
    switch (media.type) {
    case MEDIA_TYPE_FILE:
        ifile=media.src;
        break;
    default:
        break;
        return -10;
    }
    int ret=0;

    fmt_ctx=avformat_alloc_context();
    if(fmt_ctx==NULL){
        ret=-10;
        return ret;
    }

    //打开媒体文件
    av_dict_set(&fmt_opts,"buffer_size","2048000",0);
    ret = avformat_open_input(&fmt_ctx, ifile.c_str(), ifmt, &fmt_opts);
    if (ret != 0) {
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        return ret;
    }


    //查找是否有流
    ret=avformat_find_stream_info(fmt_ctx,NULL);
    if(ret!=0){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        return ret;
    }


    //获取视频流音频流索引
    video_stream_index=av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_VIDEO,-1,-1,NULL,0);
    audio_stream_index=av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    subtitle_stream_index=av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_SUBTITLE,-1,-1,NULL,0);

    if(video_stream_index<0){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        ret=-20;
        return ret;
    }


    //根据索引获取视频流
    AVStream* video_stream=fmt_ctx->streams[video_stream_index];


    //获取视频源文件编码
    AVCodecParameters* codec_param=video_stream->codecpar;

    //解码器
    const AVCodec* codec=NULL;
    codec=avcodec_find_decoder(codec_param->codec_id);
    if(codec==NULL){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        ret=-30;
        return ret;
    }

    // 解码器上下文
    codec_ctx=avcodec_alloc_context3(codec);
    if(codec_ctx==NULL){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        ret=-40;
        return ret;
    }

    // 配置解码上下文的解码参数
    ret=avcodec_parameters_to_context(codec_ctx,codec_param);
    if(ret!=0){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        if(codec_ctx){
            avcodec_free_context(&codec_ctx);
            codec_ctx=NULL;
        }
        return ret;
    }

    //配置引用计数
    if(codec_ctx->codec_type==AVMEDIA_TYPE_VIDEO||
        codec_ctx->codec_type==AVMEDIA_TYPE_AUDIO){
        av_dict_set(&codec_opts,"refcounted_frames","1",0);
    }


    //解码
    ret=avcodec_open2(codec_ctx,codec,&codec_opts);
    if(ret!=0){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        if(codec_ctx){
            avcodec_free_context(&codec_ctx);
            codec_ctx=NULL;
        }
        return ret;
    }


}
