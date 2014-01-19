TARGET   = Configuration
TEMPLATE = lib

QT      += network
QT      += script
QT      += xml

CONFIG  += staticlib
CONFIG  += debug

INCLUDEPATH += ../

SOURCES += ConfigurationScriptable.cpp \
           Device.cpp \
           Devices.cpp \
           Library.cpp \
           Libraries.cpp \
           Script.cpp \
           Scripts.cpp


HEADERS += Configuration.h \
           ConfigurationInterface.h \
           ConfigurationScriptable.h \
           Device.h \
           Devices.h \
           Library.h \
           Libraries.h \
           Script.h \
           Scripts.h
