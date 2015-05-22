TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -pedantic

SOURCES += main.cpp \
    r-macro.c

HEADERS += \
    r-macro.h \
    r-macro.hpp

