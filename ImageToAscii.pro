#-------------------------------------------------
#
# Project created by QtCreator 2015-02-13T15:16:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageToAscii
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    graphicsscene.cpp \
    converter.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    graphicsscene.h \
    converter.h

FORMS    += mainwindow.ui
