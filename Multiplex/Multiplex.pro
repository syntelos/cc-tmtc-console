TARGET   = Multiplex
TEMPLATE = lib

QT      += network
QT      += script

CONFIG  += staticlib

INCLUDEPATH += ../


SOURCES += MultiplexIndex.cpp \
           MultiplexObject.cpp \
           MultiplexRecord.cpp \
           MultiplexSelect.cpp \
           MultiplexTable.cpp \
           MultiplexTableIterator.cpp \
           MultiplexTableSearch.cpp


HEADERS += Multiplex.h \
           MultiplexIndex.h \
           MultiplexObject.h \
           MultiplexRecord.h \
           MultiplexRecordIterator.h \
           MultiplexSelect.h \
           MultiplexTable.h \
           MultiplexTableIterator.h \
           MultiplexTableSearch.h

