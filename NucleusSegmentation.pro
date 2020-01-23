#-------------------------------------------------
#
# Project created by QtCreator 2020-01-20T23:26:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NucleusSegmentation
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    unet_nucleus_seg.cpp

HEADERS  += mainwindow.h \
    unet_nucleus_seg.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:\third_lib\opencv\build\include

LIBS += D:\third_lib\opencv\build\x64\vc14\lib\opencv_world420.lib
