TARGET   = System
TEMPLATE = lib

QT      += network
QT      += script
QT      += xml

CONFIG  += staticlib

INCLUDEPATH += ../


HEADERS += SystemCatalog.h \
           SystemCatalogIO.h \
           SystemCatalogInput.h \
           SystemCatalogNode.h \
           SystemCatalogOutput.h \
           SystemCatalogProperties.h \
           SystemConnectionTCP.h \
           SystemConnector.h \
           SystemDeviceConnectionConstructorDiscovery.h \
           SystemDeviceConnectionConstructor.h \
           SystemDeviceConnection.h \
           SystemDeviceConstructorDiscovery.h \
           SystemDeviceConstructor.h \
           SystemDevice.h \
           SystemDeviceIdentifier.h \
           SystemMessage.h \
           SystemMultiplex.h \
           SystemMultiplexObject.h \
           SystemMultiplexSelect.h \
           SystemMultiplexTable.h \
           SystemMultiplexTableMem.h \
           SystemMultiplexTableConstructorDiscovery.h \
           SystemMultiplexTableConstructor.h \
           SystemName.h \
           SystemNameValue.h \
           SystemScriptable.h \
           SystemScriptSymbol.h \
           SystemTextBuffer.h


SOURCES += SystemCatalogInput.cpp \
           SystemCatalogNode.cpp \
           SystemCatalogOutput.cpp \
           SystemCatalogProperties.cpp \
           SystemConnectionTCP.cpp \
           SystemConnector.cpp \
           SystemDevice.cpp \
           SystemDeviceConnectionConstructorDiscovery.cpp \
           SystemDeviceConnection.cpp \
           SystemDeviceConstructorDiscovery.cpp \
           SystemDeviceIdentifier.cpp \
           SystemMessage.cpp \
           SystemMultiplex.cpp \
           SystemMultiplexSelect.cpp \
           SystemMultiplexTable.cpp \
           SystemMultiplexTableMem.cpp \
           SystemMultiplexTableConstructorDiscovery.cpp \
           SystemName.cpp \
           SystemNameValue.cpp \
           SystemScriptable.cpp \
           SystemScriptSymbol.cpp \
           SystemTextBuffer.cpp
