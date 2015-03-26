#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T15:58:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = test_completer
TEMPLATE = app


SOURCES += main.cpp\
        dialog_ware_name.cpp \
    line_edit_ware_name.cpp \
    ware_name_converter.cpp

HEADERS  += dialog_ware_name.h \
    line_edit_ware_name.h \
    ware_name_converter.h

RESOURCES +=
