CONFIG += -static -release -no-exceptions -no-qt3support -nomake demos -nomake examples
TARGET       = EMP
TEMPLATE     = app
QT          += core gui network

HEADERS	     = \
    src/EMP.h \
    src/VersionNo.h \
    src/VideoWidget.h \
    src/VolumeSlider.h \
    src/SeekSlider.h \
    src/SocketServer.h \
    src/Explorer.h \
    src/osdwidget.h
SOURCES	     = \
    src/main.cpp \
    src/EMP.cpp \
    src/VideoWidget.cpp \
    src/VolumeSlider.cpp \
    src/SeekSlider.cpp \
    src/SocketServer.cpp \
    src/Explorer.cpp \
    src/osdwidget.cpp

INCLUDEPATH += $$PWD/3rdparty
LIBS     += -L$$PWD/3rdparty/libvlc -lvlc

RESOURCES += \
    EMP.qrc
RC_FILE = EMPApp.rc

DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
RCC_DIR = build/rcc/

include(3rdparty/qtsingleapplication/qtsingleapplication.pri)

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8
include(lang/lang.pri)










