#ifndef AVDEF_H
#define AVDEF_H

typedef enum {
    MEDIA_TYPE_FILE=0,
    MEDIA_TYPE_NB,
    MEDIA_TYPE_NONE
} media_type_e;


typedef enum{
    PIX_FMT_NONE=0,
    PIX_FMT_IYUV//YYYY YYYY UUVV
} pix_fmt_e;


typedef enum{
    ASPECT_FULL,//100%
    ASPECT_PERCENT,         // 50%
    ASPECT_ORIGINAL_RATIO,  // w:h
    ASPECT_ORIGINAL_SIZE,   // wxh
    ASPECT_CUSTOM_RATIO,    // 4:3 16:9 ...
    ASPECT_CUSTOM_SIZE,     // 1280x720 640*480 ..
}aspect_ratio_e;

typedef struct aspect_ratio_s {
    aspect_ratio_e type;
    int w, h;
} aspect_ratio_t;


#endif // AVDEF_H
