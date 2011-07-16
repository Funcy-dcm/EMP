TEMPLATE     = app
HEADERS	     = MediaPlayer.h
SOURCES	     = MediaPlayer.cpp main.cpp
QT           += phonon
win32:TARGET = ../MediaPlayer

RESOURCES += \
    MediaPlayer.qrc

RC_FILE = MediaPlayerApp.rc
