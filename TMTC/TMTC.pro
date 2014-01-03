TARGET   = TMTC
TEMPLATE = lib

QT      -= gui
QT      += network

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += TMTCNameValue.cpp \
           TMTCName.cpp \
           TMTCMessage.cpp

HEADERS += TMTCNameValue.h \
           TMTCName.h \
           TMTCMessage.h

