QT += core network
QT -= gui
CONFIG +=c++11
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
    server.h \
    protocol.h


INCLUDEPATH+=/usr/include
LIBS +=   -lopencv_core  -lopencv_highgui  -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_video
#QMAKE_CXXFLAGS+=-v
QMAKE_CXXFLAGS+=-w
