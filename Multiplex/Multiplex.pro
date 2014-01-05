TARGET   = Multiplex
TEMPLATE = lib

QT      += network
QT      += script

CONFIG  += staticlib

INCLUDEPATH += ../


SOURCES += Multiplex.cpp \
           MultiplexDevices.cpp \
           MultiplexDeviceHistory.cpp \
           MultiplexRecord.cpp \
           MultiplexSelect.cpp \
           MultiplexTable.cpp

HEADERS += Multiplex.h \
           MultiplexDevices.h \
           MultiplexDeviceHistory.h \
           MultiplexRecord.h \
           MultiplexSelect.h \
           MultiplexTable.h
