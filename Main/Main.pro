TARGET = "Console"
TEMPLATE = app

QT      += network
QT      += sql
QT      += script
QT      += svg

INCLUDEPATH += ../

CONFIG  += staticlib
CONFIG  += debug

SOURCES += Window.cpp \
           Main.cpp

HEADERS += Window.h

unix { 
    LIBS += -L../XPORT
    LIBS += -lXPORT
    LIBS += -L../Configuration
    LIBS += -lConfiguration
    LIBS += -L../Storage
    LIBS += -lStorage
    LIBS += -L../Multiplex
    LIBS += -lMultiplex
    LIBS += -L../Terminal
    LIBS += -lTerminal
    LIBS += -L../Graphics
    LIBS += -lGraphics
    LIBS += -L../TMTC
    LIBS += -lTMTC
    LIBS += -L../ObjectTree
    LIBS += -lObjectTree
    LIBS += -L../CodeEditor
    LIBS += -lCodeEditor
    LIBS += -L../System
    LIBS += -lSystem
}
win32 { 
    RC_FILE = windows.rc
    CONFIG(debug)
    { 
        LIBS += -L../XPORT/debug
        OTHER_FILES += ../XPORT/debug/libXPORT.a
        LIBS += -L../Configuration/debug
        OTHER_FILES += ../Configuration/debug/libConfiguration.a
        LIBS += -L../Storage/debug
        OTHER_FILES += ../Storage/debug/libStorage.a
        LIBS += -L../Multiplex/debug
        OTHER_FILES += ../Multiplex/debug/libMultiplex.a
        LIBS += -L../Terminal/debug
        OTHER_FILES += ../Terminal/debug/libTerminal.a
        LIBS += -L../Graphics/debug
        OTHER_FILES += ../Graphics/debug/libGraphics.a
        LIBS += -L../TMTC/debug
        OTHER_FILES += ../TMTC/debug/libTMTC.a
        LIBS += -L../ObjectTree/debug
        OTHER_FILES += ../ObjectTree/debug/libObjectTree.a
        LIBS += -L../CodeEditor/debug
        OTHER_FILES += ../CodeEditor/debug/libCodeEditor.a
        OTHER_FILES += ../System/debug/libSystem.a
        LIBS += -L../System/debug
    }
    CONFIG(release)
    { 
        LIBS += -L../XPORT/release
        OTHER_FILES += ../XPORT/release/libXPORT.a
        LIBS += -L../Configuration/release
        OTHER_FILES += ../Configuration/release/libConfiguration.a
        LIBS += -L../Storage/release
        OTHER_FILES += ../Storage/release/libStorage.a
        LIBS += -L../Multiplex/release
        OTHER_FILES += ../Multiplex/release/libMultiplex.a
        LIBS += -L../Terminal/release
        OTHER_FILES += ../Terminal/release/libTerminal.a
        LIBS += -L../Graphics/release
        OTHER_FILES += ../Graphics/release/libGraphics.a
        LIBS += -L../TMTC/release
        OTHER_FILES += ../TMTC/release/libTMTC.a
        LIBS += -L../ObjectTree/release
        OTHER_FILES += ../ObjectTree/release/libObjectTree.a
        LIBS += -L../CodeEditor/release
        OTHER_FILES += ../CodeEditor/release/libCodeEditor.a
        OTHER_FILES += ../System/release/libSystem.a
        LIBS += -L../System/release
    }

    LIBS += -lXPORT
    LIBS += -lConfiguration
    LIBS += -lStorage
    LIBS += -lMultiplex
    LIBS += -lTerminal
    LIBS += -lGraphics
    LIBS += -lTMTC
    LIBS += -lObjectTree
    LIBS += -lCodeEditor
    LIBS += -lSystem

    RC_FILE = windows.rc
}

RESOURCES += resources.qrc
OTHER_FILES += windows.rc
