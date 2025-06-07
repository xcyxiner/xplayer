#include "hframebuf.h"

HFrameBuf::HFrameBuf() {
    cache_num=DEFAUTL_FRAME_CACHENUM;
}

int HFrameBuf::push(HFrame *pFrame)
{
    std::unique_lock<std::mutex> locker(this->mutex);
    if(pFrame->isNull())
        return -10;

    this->buffer_not_full.wait(locker,[this](){
        return frames.size() < cache_num;
    });

    if(frames.size()>=(size_t)cache_num){
        HFrame frame = std::move(frames.front()); // 移动所有权
        frames.pop_front();  // 先移除，避免析构冲突

        free(frame.buf.len);   // 安全释放内存
        frame.buf = {nullptr, 0}; // 必须置空!

        if (frame.userdata) {
            ::free(frame.userdata);
            frame.userdata = nullptr;
        }
    }
    int ret=0;
    if(isNull()){
        resize(pFrame->buf.len*cache_num);
        ret=1;
    }

    HFrame frame;
    frame.buf.base=alloc(pFrame->buf.len);
    frame.buf.len=pFrame->buf.len;
    frame.copy(*pFrame);
    frames.push_back(frame);

    locker.unlock();
    this->buffer_not_empty.notify_one();
    return ret;
}

int HFrameBuf::pop(HFrame *pFrame)
{
    std::unique_lock<std::mutex> locker(this->mutex);

    if(isNull())
        return -10;


    if(frames.size()==0){
        return -20;
    }

    this->buffer_not_empty.wait(locker, [this] {
        return  !frames.empty();
    });


    HFrame frame = std::move(frames.front());
    frames.pop_front();

    if(frame.isNull()){
        return -30;
    }

    pFrame->copy(frame);

    free(frame.buf.len);
    frame.buf={nullptr,0};

    if(frame.userdata){
        ::free(frame.userdata);
        frame.userdata=nullptr;
    }

    locker.unlock();
    this->buffer_not_full.notify_one();

    return 0;
}

void HFrameBuf::clear()
{
    std::unique_lock<std::mutex> locker(this->mutex);
    frames.clear();
    HRingBuf::clear();
}
