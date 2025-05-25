#ifndef HRINGBUF_H
#define HRINGBUF_H

#include "hbuf.h"

class HRingBuf : public HBuf
{
public:
    HRingBuf();
    HRingBuf(size_t cap);

    virtual ~HRingBuf(){

    }

    char* alloc(size_t len);
    void free(size_t len);
    void clear();

    size_t size() const;

private:
    size_t _head;
    size_t _tail;
    size_t _size;
};

#endif // HRINGBUF_H
