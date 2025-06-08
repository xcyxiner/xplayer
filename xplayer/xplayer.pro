QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT +=widgets openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiownd.cpp \
    glwnd.cpp \
    haudiownd.cpp \
    havframebuf.cpp \
    hbuf.cpp \
    hffplayer.cpp \
    hframe.cpp \
    hframebuf.cpp \
    hmultiview.cpp \
    hopenmediadlg.cpp \
    hringbuf.cpp \
    hthread.cpp \
    hvideoplayer.cpp \
    hvideowidget.cpp \
    hvideownd.cpp \
    hglwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    sdl2audiownd.cpp

HEADERS += \
    audiownd.h \
    avdef.h \
    ffmpeg_util.h \
    glwnd.h \
    haudiownd.h \
    havframebuf.h \
    hbuf.h \
    hffplayer.h \
    hframe.h \
    hframebuf.h \
    hmedia.h \
    hmultiview.h \
    hopenmediadlg.h \
    hringbuf.h \
    hthread.h \
    hvideoplayer.h \
    hvideowidget.h \
    hvideownd.h \
    hglwidget.h\
    mainwindow.h \
    sdl2audiownd.h

FORMS += \
    hmultiview.ui \
    hopenmediadlg.ui \
    hvideowidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH +=$$PWD/3rd/include
INCLUDEPATH +=$$PWD/3rd/include/GL
INCLUDEPATH +=$$PWD/3rd/include/sdl2
win32 {
    win32-msvc{
        LIBS += -lavformat      \
                -lavdevice      \
                -lavcodec       \
                -lswresample    \
                -lswscale       \
                -lavutil        \

        LIBS += -lopengl32 -lglu32
        LIBS += -lglew32
        LIBS += -lSDL2

        DESTDIR = $$PWD/3rd/bin/msvc
        LIBS += -L$$PWD/3rd/lib/msvc


    }
}
