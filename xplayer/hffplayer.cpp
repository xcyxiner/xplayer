#include "hffplayer.h"
#include <iostream>

HFFPlayer::HFFPlayer() {

    fmt_opts=NULL;
    fmt_ctx=NULL;
    codec_ctx = NULL;
    codec_opts=NULL;
}

int HFFPlayer::start()
{
    quit=0;
    HThread::start();
    return 0;
}

bool HFFPlayer::doPrepare()
{
    int ret=open();
    return true;
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

    // 原始视频宽高以及像素格式
    int sw,sh,dw,dh;
    sw=codec_ctx->width;
    sh=codec_ctx->height;
    src_pix_fmt=codec_ctx->pix_fmt;
    if(sw<=0|| sh <=0 || src_pix_fmt==AV_PIX_FMT_NONE){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        if(codec_ctx){
            avcodec_free_context(&codec_ctx);
            codec_ctx=NULL;
        }
        ret=-45;
        return ret;
    }

    //目标视频宽高以及像素格式
    dw=sw&(~3);
    dh=sh&(~3);
    dst_pix_fmt=AV_PIX_FMT_YUV420P;

    //图像缩放以及像素格式转换
    sws_ctx=sws_getContext(sw,sh,src_pix_fmt,dw,dh,dst_pix_fmt,SWS_BICUBIC,NULL,NULL,NULL);
    if(sws_ctx==NULL){
        if(fmt_ctx){
            avformat_close_input(&fmt_ctx);
            avformat_free_context(fmt_ctx);
            fmt_ctx=NULL;
        }
        if(codec_ctx){
            avcodec_free_context(&codec_ctx);
            codec_ctx=NULL;
        }
        ret=-50;
        return ret;
    }


    //视频帧存储
    packet=av_packet_alloc();
    frame=av_frame_alloc();

    hframe.w=dw;
    hframe.h=dh;

    hframe.buf.resize(dw*dh*4);

    if(dst_pix_fmt==AV_PIX_FMT_YUV420P){
        hframe.type=PIX_FMT_IYUV;
        hframe.bpp=12;
        int y_size=dw*dh;
        hframe.buf.len=y_size*3/2;

        data[0]=(uint8_t*)hframe.buf.base;
        data[1]=data[0]+y_size;
        data[2]=data[1]+y_size/4;
        linesize[0]=dw;
        linesize[1]=linesize[2]=dw/2;
    }

    if (video_stream->avg_frame_rate.num && video_stream->avg_frame_rate.den) {
        fps = video_stream->avg_frame_rate.num / video_stream->avg_frame_rate.den;
    }

    return ret;
}

void HFFPlayer::doTask()
{
    while (!this->quit) {
        av_init_packet(packet);

        int ret=av_read_frame(this->fmt_ctx,this->packet);

        if(ret!=0){
            if(!this->quit){

            }
            av_packet_unref(this->packet);
            return;
        }


        if(this->packet->stream_index!= this->video_stream_index){
            continue;
        }


        ret=avcodec_send_packet(this->codec_ctx,this->packet);
        if(ret!=0){
            av_packet_unref(this->packet);
            return;
        }

        ret= avcodec_receive_frame(this->codec_ctx,this->frame);
        if(ret!=0){

        }else{
            break;
        }
    }

    if(this->sws_ctx){
        int h=sws_scale(this->sws_ctx,this->frame->data,
                          this->frame->linesize,0,this->frame->height,this->data,this->linesize);
        if(h<=0 || h != this->frame->height){
            return;
        }
    }

    this->push_frame(&this->hframe);
}

int HFFPlayer::stop()
{
    return HThread::stop();
}

int HFFPlayer::pause()
{
 return HThread::pause();
}

int HFFPlayer::resume()
{
 return HThread::resume();
}
