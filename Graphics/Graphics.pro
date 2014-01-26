TARGET   = Graphics
TEMPLATE = lib

QT      += script
QT      += network
QT      += xml

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += GraphicsBranch.cpp \
           GraphicsCanvas.cpp \
           GraphicsScene.cpp \
           GraphicsTerminal.cpp \
           GraphicsTerminalInput.cpp \
           GraphicsTerminalOutput.cpp \
           GraphicsTerminalText.cpp

HEADERS += GraphicsLeaf.h \
           GraphicsBranch.h \
           GraphicsCanvas.h \
           GraphicsScene.h \
           GraphicsTerminal.h \
           GraphicsTerminalInput.h \
           GraphicsTerminalOutput.h \
           GraphicsTerminalText.h

