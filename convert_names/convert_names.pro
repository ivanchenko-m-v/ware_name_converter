#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T14:57:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../
TARGET = convert_names
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../ware_name_text.cpp

HEADERS += \
    ../ware_name_text.h
