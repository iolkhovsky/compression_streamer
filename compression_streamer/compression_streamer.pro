QT -= gui

CONFIG += c++1z console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arg_parser.cpp \
    configurator.cpp \
    ipc_manager.cpp \
        main.cpp \
    posix_mqueue.cpp \
    posix_semaphore.cpp \
    posix_shmem.cpp \
        stream_timing_stat.cpp \
        stream_traffic_stat.cpp \
    transport_protocol.cpp \
    video_codec.cpp \
        video_source.cpp \
        video_streamer.cpp \
    video_receiver.cpp

INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_videoio \
        -lopencv_imgproc \
        -lopencv_imgcodecs

LIBS += -lrt


HEADERS += \
    arg_parser.h \
    configurator.h \
    ipc_manager.h \
    posix_mqueue.h \
    posix_semaphore.h \
    posix_shmem.h \
    stream_timing_stat.h \
    stream_traffic_stat.h \
    synchronized.h \
    transport_protocol.h \
    video_codec.h \
    video_source.h \
    video_streamer.h \
    paginator.h \
    video_receiver.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
