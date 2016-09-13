#-------------------------------------------------
#
# Project created by QtCreator 2016-09-09T04:44:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network

TARGET = tpchat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    chatwindow.cpp \
    personList.cpp \
    personlistbuddy.cpp

HEADERS  += mainwindow.h \
    login.h \
    chatwindow.h \
    personList.h \
    personlistbuddy.h

