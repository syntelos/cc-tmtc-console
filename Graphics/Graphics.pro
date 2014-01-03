TARGET   = Graphics
TEMPLATE = lib

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += GraphicsBranch.cpp \
           GraphicsBody.cpp \
           GraphicsScene.cpp

HEADERS += GraphicsLeaf.h \
           GraphicsBranch.h \
           GraphicsBody.h \
           GraphicsScene.h

