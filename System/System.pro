TARGET   = System
TEMPLATE = lib

QT      += script
QT      += network
QT      += xml

CONFIG  += staticlib

INCLUDEPATH += ../


HEADERS += SystemCatalog.h \
           SystemCatalogInput.h \
           SystemCatalogNode.h \
           SystemCatalogOutput.h \
           SystemCatalogProperties.h \
           SystemConnector.h \
           SystemDevice.h \
           SystemDeviceConnection.h \
           SystemDeviceIdentifier.h \
           SystemScriptable.h \
           SystemScriptSymbol.h \
           SystemTextBuffer.h

SOURCES += SystemCatalogInput.cpp \
           SystemCatalogNode.cpp \
           SystemCatalogOutput.cpp \
           SystemCatalogProperties.cpp \
           SystemConnector.cpp \
           SystemDeviceConnection.cpp \
           SystemDeviceIdentifier.cpp \
           SystemScriptable.cpp \
           SystemScriptSymbol.cpp \
           SystemTextBuffer.cpp
