TARGET = qht1
TEMPLATE = app

QT     += network

CONFIG += debug

INCLUDEPATH += ../

SOURCES += Main.cpp

unix {
    LIBS += -L../TMTC
    LIBS += -lTMTC
    LIBS += -L../System
    LIBS += -lSystem
}
