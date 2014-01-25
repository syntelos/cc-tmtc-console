TARGET = ../tmtc-console
TEMPLATE = app

QT      += network
QT      += sql
QT      += script
QT      += xml

INCLUDEPATH += ../

CONFIG  += staticlib
CONFIG  += debug

SOURCES += Window.cpp \
           Main.cpp

HEADERS += Window.h

unix { 
    LIBS += -L../Configuration
    LIBS += -lConfiguration
    LIBS += -L../Storage
    LIBS += -lStorage
    LIBS += -L../Terminal
    LIBS += -lTerminal
    LIBS += -L../Graphics
    LIBS += -lGraphics
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
        LIBS += -L../Configuration/debug
        OTHER_FILES += ../Configuration/debug/libConfiguration.a
        LIBS += -L../Storage/debug
        OTHER_FILES += ../Storage/debug/libStorage.a
        LIBS += -L../Terminal/debug
        OTHER_FILES += ../Terminal/debug/libTerminal.a
        LIBS += -L../Graphics/debug
        OTHER_FILES += ../Graphics/debug/libGraphics.a
        LIBS += -L../ObjectTree/debug
        OTHER_FILES += ../ObjectTree/debug/libObjectTree.a
        LIBS += -L../CodeEditor/debug
        OTHER_FILES += ../CodeEditor/debug/libCodeEditor.a
        OTHER_FILES += ../System/debug/libSystem.a
        LIBS += -L../System/debug
    }
    CONFIG(release)
    { 
        LIBS += -L../Configuration/release
        OTHER_FILES += ../Configuration/release/libConfiguration.a
        LIBS += -L../Storage/release
        OTHER_FILES += ../Storage/release/libStorage.a
        LIBS += -L../Terminal/release
        OTHER_FILES += ../Terminal/release/libTerminal.a
        LIBS += -L../Graphics/release
        OTHER_FILES += ../Graphics/release/libGraphics.a
        LIBS += -L../ObjectTree/release
        OTHER_FILES += ../ObjectTree/release/libObjectTree.a
        LIBS += -L../CodeEditor/release
        OTHER_FILES += ../CodeEditor/release/libCodeEditor.a
        OTHER_FILES += ../System/release/libSystem.a
        LIBS += -L../System/release
    }

    LIBS += -lConfiguration
    LIBS += -lStorage
    LIBS += -lTerminal
    LIBS += -lGraphics
    LIBS += -lObjectTree
    LIBS += -lCodeEditor
    LIBS += -lSystem

    RC_FILE = windows.rc
}

RESOURCES += resources.qrc
OTHER_FILES += windows.rc
