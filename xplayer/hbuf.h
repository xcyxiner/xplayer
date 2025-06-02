#ifndef HBUF_H
#define HBUF_H

#include <cstdlib>
class HBuf
{
public:
    char* base;
    size_t len;
    HBuf()  {
        base=NULL;
        len=0;
        cleanup_ = false;
    }
    HBuf(void* data, size_t len)  {
        this->base=(char*)data;
        this->len=len;
        cleanup_ = false;
    }

    HBuf(size_t cap){
        resize(cap);
    }

    ~HBuf(){
        if (base) {
            // free(base);
            base = NULL;
        }
    }
    bool isNull();

    char *getBase() const;

    size_t getLen() const;

    void resize(size_t cap);

    void copy(void* data,size_t len);

private:
    bool cleanup_;
};

#endif // HBUF_H
