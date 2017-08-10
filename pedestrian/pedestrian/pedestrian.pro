QT += core
QT -= gui

TARGET = pedestrian
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    config.cpp

HEADERS += \
    common.h \
    config.h

