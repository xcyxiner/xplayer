#ifndef AVDEF_H
#define AVDEF_H

typedef enum {
    MEDIA_TYPE_FILE=0,
    MEDIA_TYPE_NB,
    MEDIA_TYPE_NONE
} media_type_e;


typedef enum{
    PIX_FMT_NONE = 0,

    PIX_FMT_GRAY,       // YYYYYYYY

    PIX_FMT_YUV_FIRST = 100,
    PIX_FMT_YUV_PLANAR_FIRST = 200,
    PIX_FMT_IYUV,       // YYYYYYYYUUVV
    PIX_FMT_YV12,       // YYYYVVYYVVUU
    PIX_FMT_NV12,       // YYUVYYYYUVUV
    PIX_FMT_NV21,       // YYVUYYYYVUVU
    PIX_FMT_YUV_PLANAR_LAST,
    PIX_FMT_YUV_PACKED_FIRST = 300,
    PIX_FMT_YUY2,       // YUYVYUYV
    PIX_FMT_YVYU,       // YVYUYVYU
    PIX_FMT_UYVY,       // UYVYUYVY
    PIX_FMT_YUV_PACKED_LAST,
    PIX_FMT_YUV_LAST,

    PIX_FMT_RGB_FIRST = 400,
    PIX_FMT_RGB,        // RGBRGB
    PIX_FMT_BGR,        // BGRBGR
    PIX_FMT_RGBA,       // RGBARGBA
    PIX_FMT_BGRA,       // BGRABGRA
    PIX_FMT_ARGB,       // ARGBARGB
    PIX_FMT_ABGR,       // ABGRABGR
    PIX_FMT_RGB_LAST,
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
