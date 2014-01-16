TARGET   = System
TEMPLATE = lib

QT      += script
QT      += network

CONFIG  += staticlib

INCLUDEPATH += ../


HEADERS += SystemDeviceConnection.h \
           SystemDevice.h \
           SystemDeviceIdentifier.h \
           SystemScriptSymbol.h \
           SystemScriptable.h \
           SystemTextBuffer.h

SOURCES += SystemScriptSymbol.cpp \
           SystemScriptable.cpp \
           SystemDeviceIdentifier.cpp \
           SystemDeviceConnection.cpp \
           SystemTextBuffer.cpp
