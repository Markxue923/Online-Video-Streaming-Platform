#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T13:06:02
#
#-------------------------------------------------

QT       += core gui
QT       += network
CONFIG  += mobility
MOBILITY += multimedia
QT  += multimediawidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    protocol.cpp \
    subwidget.cpp \
    anchor.cpp \
    clientwidget.cpp \
    videosurface.cpp \
    pack.cpp

HEADERS  += widget.h \
    protocol.h \
    subwidget.h \
    anchor.h \
    clientwidget.h \
    videosurface.h \
    pack.h

FORMS    += widget.ui \
    subwidget.ui \
    anchor.ui \
    clientwidget.ui
