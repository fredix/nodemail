#-------------------------------------------------
#
# Project created by QtCreator 2013-02-21T16:20:35
#
#-------------------------------------------------

QT       += core \
        network \
        xml \
        testlib

QT       -= gui
CONFIG += qxt
QXT     += core
QXT     += network


TARGET = nodemail
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    service.cpp \
    nodemail.cpp

HEADERS += \
    main.h \
    service.h \
    nodemail.h



LIBS += -lz \
        -lboost_system \
        -lboost_filesystem-mt \
        -lboost_thread-mt \
        -lQxtCore

INCLUDEPATH += ./externals/libqxt/
INCLUDEPATH += ./externals/libqxt/src/core
