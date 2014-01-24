TEMPLATE = lib
TARGET = HTTPStream
DEPENDPATH += .
INCLUDEPATH += .

QT += network
CONFIG += staticlib
CONFIG += debug

HEADERS += HTTPStreamChunk.h \
           HTTPStreamClient.h \
           HTTPStreamHeader.h \
           HTTPStreamIO.h \
           HTTPStreamRequest.h \
           HTTPStreamResponse.h
SOURCES += HTTPStreamChunk.cpp \
           HTTPStreamClient.cpp \
           HTTPStreamIO.cpp \
           HTTPStreamRequest.cpp \
           HTTPStreamResponse.cpp
