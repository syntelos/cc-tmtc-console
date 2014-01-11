TARGET   = Multiplex
TEMPLATE = lib

QT      += network
QT      += script

CONFIG  += staticlib

INCLUDEPATH += ../


SOURCES += Multiplex.cpp \
           MultiplexIndex.cpp \
           MultiplexObject.cpp \
           MultiplexRecord.cpp \
           MultiplexRecordIterator.cpp \
           MultiplexSelect.cpp \
           MultiplexTable.cpp \
           MultiplexTableIterator.cpp \
           MultiplexTables.cpp


HEADERS += Multiplex.h \
           MultiplexIndex.h \
           MultiplexObject.h \
           MultiplexRecord.h \
           MultiplexRecordIterator.h \
           MultiplexSelect.h \
           MultiplexTable.h \
           MultiplexTableIterator.h \
           MultiplexTables.h
