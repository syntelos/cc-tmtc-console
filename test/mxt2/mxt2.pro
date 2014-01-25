TARGET   = mxt2
TEMPLATE = app

QT      += network
QT      += script

CONFIG  += debug

INCLUDEPATH += ../

SOURCES += Main.cpp

unix {
    LIBS += -L../Multiplex
    LIBS += -lMultiplex
    LIBS += -L../TMTC
    LIBS += -lTMTC
    LIBS += -L../System
    LIBS += -lSystem
}
