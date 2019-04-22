#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T11:02:50
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    clientsocket.cpp \
    socketmanager.cpp \
    protocol.cpp \
    dbmanager.cpp \
    sqldao.cpp \
    user.cpp \
    sqldaoimp.cpp \
    room.cpp

HEADERS  += widget.h \
    clientsocket.h \
    socketmanager.h \
    protocol.h \
    dbmanager.h \
    sqldao.h \
    user.h \
    sqldaoimp.h \
    room.h

FORMS    += widget.ui
