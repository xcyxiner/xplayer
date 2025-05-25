#include "hringbuf.h"

HRingBuf::HRingBuf():HBuf() {
     _head=_tail=_size=0;
}

HRingBuf::HRingBuf(size_t cap):HBuf(cap)
{
    _head=_tail=_size=0;
}

char *HRingBuf::alloc(size_t len)
{
    char* ret=NULL;
    if(_head<_tail || _size==0){
        // [_tail, this->len) && [0, _head)
        if(this->len -_tail>=len){
            ret = base + _tail;
            _tail += len;
            if (_tail == this->len) _tail = 0;
        }
        else if(_head>=len){
            ret=base;
            _tail=len;
        }
    }else{
        // [_tail, _head)
        if(_head-_tail>=len){
            ret=base+_tail;
            _tail+=len;
        }
    }
    _size+=ret?len:0;
    return ret;
}

void HRingBuf::free(size_t len)
{
    _size-=len;
    if(len <= this->len -_head){
        _head+=len;
        if(_head==this->len)_head=0;
    }else{
        _head=len;
    }
}

void HRingBuf::clear()
{
    _head=_tail=_size=0;
}

size_t HRingBuf::size() const
{
    return _size;
}
