#-------------------------------------------------
#
# Project created by QtCreator 2014-02-13T21:25:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ledManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ledbar.cpp \
    tcpclient.cpp \
    udpclient.cpp

HEADERS  += mainwindow.h \
    ledbar.h \
    tcpclient.h \
    udpclient.h

FORMS    += mainwindow.ui
