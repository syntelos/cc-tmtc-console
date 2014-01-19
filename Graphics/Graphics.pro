TARGET   = Graphics
TEMPLATE = lib

QT      += script
QT      += network
QT      += xml

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += GraphicsBranch.cpp \
           GraphicsCanvas.cpp \
           GraphicsScene.cpp

HEADERS += GraphicsLeaf.h \
           GraphicsBranch.h \
           GraphicsCanvas.h \
           GraphicsScene.h

