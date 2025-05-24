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

#endif // AVDEF_H
