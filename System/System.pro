TARGET   = System
TEMPLATE = lib

QT      += script
QT      += network

CONFIG  += staticlib

INCLUDEPATH += ../


HEADERS += SystemDeviceConnection.h \
           SystemDevice.h \
           SystemDeviceIdentifier.h \
           SystemLibraryInterface.h \
           SystemScriptSymbol.h \
           SystemTMTCLanguage.h \
           SystemTMTCName.h \
           SystemTMTCValue.h \
           SystemWindow.h \
           SystemScriptable.h

SOURCES += SystemScriptSymbol.cpp \
           SystemScriptable.cpp \
           SystemDeviceIdentifier.cpp \
           SystemDeviceConnection.cpp
