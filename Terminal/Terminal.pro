TARGET   = Terminal
TEMPLATE = lib

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += TerminalInput.cpp \
           TerminalOutput.cpp \
           TerminalText.cpp \
           Terminal.cpp

HEADERS += TerminalInput.h \
           TerminalOutput.h \
           TerminalText.h \
           Terminal.h
