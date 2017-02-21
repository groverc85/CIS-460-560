QT       += core gui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += include
INCLUDEPATH += $$PWD

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cis277_hw01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    polygon.cpp \
    rasterizer.cpp \
    tiny_obj_loader.cc \
    mat4.cpp \
    vec4.cpp

HEADERS  += mainwindow.h \
    polygon.h \
    mat4.h \
    vec4.h \
    rasterizer.h \
    tiny_obj_loader.h

FORMS    += mainwindow.ui
