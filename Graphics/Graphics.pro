TARGET   = Graphics
TEMPLATE = lib

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += GraphicsBranch.cpp \
           GraphicsCanvas.cpp \
           GraphicsScene.cpp

HEADERS += GraphicsLeaf.h \
           GraphicsBranch.h \
           GraphicsCanvas.h \
           GraphicsScene.h

