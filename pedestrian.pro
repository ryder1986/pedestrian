QT += core network
QT -= gui

TARGET = pedestrian
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    config.cpp \
    camera.cpp \
    server.cpp

HEADERS += \
    common.h \
    config.h \
    camera.h \
    server.h


#INCLUDEPATH+=opencv249/include
LIBS +=   -lopencv_core  -lopencv_highgui  -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_video
