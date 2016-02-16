QT += core
QT -= gui

CONFIG += c++11

TARGET = FlyToEleph
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    decisiontree.cpp

HEADERS += \
    dictionary.h \
    tree.hh \
    decisiontree.h
