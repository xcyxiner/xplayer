#include "havframebuf.h"

HAVFrameBuf::HAVFrameBuf() {
     cache_num=DEFAUTL_FRAME_CACHENUM;
}

int HAVFrameBuf::push(AVFrame *pFrame)
{
    std::unique_lock<std::mutex> locker(this->mutex);
    if(!pFrame || !pFrame->data[0] || pFrame->nb_samples <= 0)
        return -10;

    this->buffer_not_full.wait(locker,[this](){
        return frames.size() < cache_num;
    });

    if(frames.size()>=(size_t)cache_num){
        AVFrame* oldFrame =frames.front(); // 移动所有权
        frames.pop_front();  // 先移除，避免析构冲突
        av_frame_free(&oldFrame);
    }
    int ret=0;

    AVFrame* newFrame = av_frame_alloc();
    if (!newFrame) {
        return -1; // 分配失败
    }

    // 复制帧内容（不复制数据，只增加引用计数）
    if (av_frame_ref(newFrame, pFrame) < 0) {
        av_frame_free(&newFrame);
        return -2; // 复制失败
    }

    if (frames.empty()) { // 或者根据你的逻辑判断是否为空
        ret = 1;
    }
    frames.push_back(newFrame);

    locker.unlock();
    this->buffer_not_empty.notify_one();
    return ret;
}

int HAVFrameBuf::pop(AVFrame *pFrame)
{
    std::unique_lock<std::mutex> locker(this->mutex);


    if(frames.size()==0){
        return -20;
    }

    this->buffer_not_empty.wait(locker, [this] {
        return  !frames.empty();
    });


    AVFrame* oldFrame =frames.front(); // 移动所有权
    frames.pop_front();  // 先移除，避免析构冲突

    if(frames.empty()){
        return -30;
    }

    // 复制帧内容（不复制数据，只增加引用计数）
    if (av_frame_ref(pFrame,oldFrame) < 0) {
        av_frame_free(&oldFrame);
        return -2; // 复制失败
    }

    locker.unlock();
    this->buffer_not_full.notify_one();

    return 0;
}

void HAVFrameBuf::clear()
{
    std::unique_lock<std::mutex> locker(this->mutex);
    frames.clear();
}
