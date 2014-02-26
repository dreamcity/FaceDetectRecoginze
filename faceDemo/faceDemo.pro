#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T14:18:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = faceDemo
TEMPLATE = app


SOURCES += main.cpp\
        detect2recognize.cpp \
    detect.cpp \
    recognize.cpp

HEADERS  += detect2recognize.h \
    detect.h \
    recognize.h

FORMS    += detect2recognize.ui \
    detect.ui \
    recognize.ui

INCLUDEPATH+= /usr/local/include
INCLUDEPATH+=/usr/local/include/opencv
INCLUDEPATH+=/usr/local/include/opencv2


LIBS += /usr/local/lib/libopencv_highgui.so.2.4.6
LIBS += /usr/local/lib/libopencv_legacy.so.2.4.6
LIBS += /usr/local/lib/libopencv_imgproc.so.2.4.6
LIBS += /usr/local/lib/libopencv_flann.so.2.4.6
LIBS += /usr/local/lib/libopencv_ts.so.2.4.6
LIBS += /usr/local/lib/libopencv_videostab.so.2.4.6
LIBS += /usr/local/lib/libopencv_ml.so.2.4.6
LIBS += /usr/local/lib/libopencv_core.so.2.4.6
LIBS += /usr/local/lib/libopencv_photo.so.2.4.6
LIBS += /usr/local/lib/libopencv_contrib.so.2.4.6
LIBS += /usr/local/lib/libopencv_video.so.2.4.6
LIBS += /usr/local/lib/libopencv_stitching.so.2.4.6
LIBS += /usr/local/lib/libopencv_objdetect.so.2.4.6
LIBS += /usr/local/lib/libopencv_nonfree.so.2.4.6
LIBS += /usr/local/lib/libopencv_calib3d.so.2.4.6
LIBS += /usr/local/lib/libopencv_features2d.so.2.4.6
