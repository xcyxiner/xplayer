#ifndef HMEDIA_H
#define HMEDIA_H
#include "avdef.h"
#include <string>
typedef struct media_s{
    media_type_e type;
    std::string src;
    std::string descr;
    int index;

    media_s(){
        type=MEDIA_TYPE_NONE;
        index=-1;
    }
} HMedia;
#endif // HMEDIA_H
