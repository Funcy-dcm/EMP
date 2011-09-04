#CONFIG += release
TEMPLATE     = app
TARGET       = EMP
QT          += phonon\
                network

HEADERS	     = \
    src/EMP.h \
    src/qtsingleapplication/qtsingleapplication.h \
    src/qtsingleapplication/qtlockedfile.h \
    src/qtsingleapplication/qtlocalpeer.h
SOURCES	     = \
    src/main.cpp \
    src/EMP.cpp \
    src/qtsingleapplication/qtsingleapplication.cpp \
    src/qtsingleapplication/qtlockedfile_win.cpp \
    src/qtsingleapplication/qtlockedfile_unix.cpp \
    src/qtsingleapplication/qtlockedfile.cpp \
    src/qtsingleapplication/qtlocalpeer.cpp

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
