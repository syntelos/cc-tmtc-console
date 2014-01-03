TARGET   = Configuration
TEMPLATE = lib

QT      += network
QT      += sql
QT      += script

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += Configuration.cpp \
           ConfigurationScriptable.cpp \
           CCDB.cpp \
           HCDB.cpp \
           Device.cpp \
           Devices.cpp \
           Library.cpp \
           Libraries.cpp \
           Script.cpp \
           Scripts.cpp


HEADERS += Configuration.h \
           ConfigurationInterface.h \
           ConfigurationError.h \
           ConfigurationScriptable.h \
           CCDB.h \
           HCDB.h \
           Device.h \
           Devices.h \
           Library.h \
           Libraries.h \
           Script.h \
           Scripts.h
