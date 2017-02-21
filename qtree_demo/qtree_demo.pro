#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T11:35:44
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtree_demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mytreewidget.cpp \
    node.cpp

HEADERS  += mainwindow.h \
    mytreewidget.h \
    node.h

FORMS    += mainwindow.ui
