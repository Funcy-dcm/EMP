QT          += core gui network

TARGET       = EMP
TEMPLATE     = app

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

CONFIG(debug, debug|release) {
  BUILD_DIR = debug
} else {
  BUILD_DIR = release
  DEFINES += QT_NO_DEBUG_OUTPUT
}

DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
RCC_DIR = build/rcc/

INCLUDEPATH += $$PWD/3rdparty
LIBS     += -L$$PWD/3rdparty/libvlc -lvlc

RESOURCES += \
    EMP.qrc
RC_FILE = EMPApp.rc

include(3rdparty/qtsingleapplication/qtsingleapplication.pri)

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8
include(lang/lang.pri)










