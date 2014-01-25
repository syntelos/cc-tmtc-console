TARGET = qht0
TEMPLATE = app

QT     += network

CONFIG += debug

INCLUDEPATH += ../

SOURCES += Main.cpp

unix {
    LIBS += -L../System
    LIBS += -lSystem
}
