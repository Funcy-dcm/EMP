CONFIG += -static -release -no-exceptions -no-qt3support -nomake demos -nomake examples
TARGET       = EMP
TEMPLATE     = app
QT          += core gui network

HEADERS	     = \
    src/EMP.h \
    src/qtsingleapplication/qtsingleapplication.h \
    src/qtsingleapplication/qtlockedfile.h \
    src/qtsingleapplication/qtlocalpeer.h \
    src/VersionNo.h \
    src/VideoWidget.h \
    src/VolumeSlider.h \
    src/SeekSlider.h \
    src/SocketServer.h
SOURCES	     = \
    src/main.cpp \
    src/EMP.cpp \
    src/qtsingleapplication/qtsingleapplication.cpp \
    src/qtsingleapplication/qtlockedfile_win.cpp \
    src/qtsingleapplication/qtlockedfile_unix.cpp \
    src/qtsingleapplication/qtlockedfile.cpp \
    src/qtsingleapplication/qtlocalpeer.cpp \
    src/VideoWidget.cpp \
    src/VolumeSlider.cpp \
    src/SeekSlider.cpp \
    src/SocketServer.cpp

LIBS     += -L$$PWD/libvlc -lvlc

RESOURCES += \
    EMP.qrc
RC_FILE = EMPApp.rc

DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
RCC_DIR = build/rcc/

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8
include(lang/lang.pri)








