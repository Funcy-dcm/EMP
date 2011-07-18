TEMPLATE     = app
HEADERS	     = EMP.h
SOURCES	     = EMP.cpp main.cpp
QT           += phonon
win32:TARGET = ../EMP

RESOURCES += \
    EMP.qrc

RC_FILE = EMPApp.rc

TRANSLATIONS    = Lang/ru.ts \
                  Lang/en.ts \
                  Lang/de.ts \
