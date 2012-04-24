QT          += core gui network

TARGET       = EMP
TEMPLATE     = app

HEADERS += \
    src/volumeslider.h \
    src/videowidget.h \
    src/socketserver.h \
    src/seekslider.h \
    src/osdwidget.h \
    src/explorer.h \
    src/emp.h \
    src/appversion.h

SOURCES += \
    src/volumeslider.cpp \
    src/videowidget.cpp \
    src/socketserver.cpp \
    src/osdwidget.cpp \
    src/main.cpp \
    src/explorer.cpp \
    src/emp.cpp \
    src/seekslider.cpp

CONFIG(debug, debug|release) {
  BUILD_DIR = debug
} else {
  BUILD_DIR = release
  DEFINES += QT_NO_DEBUG_OUTPUT
}

DESTDIR = $${BUILD_DIR}/target/
OBJECTS_DIR = $${BUILD_DIR}/obj/
MOC_DIR = $${BUILD_DIR}/moc/
RCC_DIR = $${BUILD_DIR}/rcc/

include(3rdparty/qtsingleapplication/qtsingleapplication.pri)

INCLUDEPATH += $$PWD/3rdparty
LIBS     += -L$$PWD/3rdparty/libvlc -lvlc

RESOURCES += \
    emp.qrc
RC_FILE = emp.rc

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8
include(lang/lang.pri)









