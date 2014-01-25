TARGET   = Multiplex
TEMPLATE = lib

QT      += network
QT      += script
QT      += xml

CONFIG  += staticlib

INCLUDEPATH += ../


SOURCES += Multiplex.cpp \
           MultiplexIndex.cpp \
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

