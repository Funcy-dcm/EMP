#CONFIG += release
TEMPLATE     = app
HEADERS	     = EMP.h
SOURCES	     = EMP.cpp main.cpp
QT           += phonon\
                network
win32:TARGET = ../EMP

RESOURCES += \
    EMP.qrc

RC_FILE = EMPApp.rc

CODECFORTR = UTF-8
CODECFORSRC = UTF-8
include(lang/lang.pri)
