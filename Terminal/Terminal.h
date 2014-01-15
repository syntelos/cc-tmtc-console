/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_TERMINAL_H
#define _CONSOLE_TERMINAL_H

#include <QGraphicsItem>

#include "Graphics/GraphicsBranch.h"
#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "TerminalOutput.h"
#include "TerminalInput.h"

/*!
 * Combines terminal output and input into one component
 */
class Terminal : public GraphicsBranch
{
    Q_OBJECT;

    static const char* TerminalClassName;

    TerminalOutput output;
    TerminalInput input;

 public:
    Terminal(QGraphicsItem *parent = 0);
    ~Terminal();

    int getFontSize();
    void setFontSize(int);

    int getRows();
    void setRows(int);

    int getColumns();
    void setColumns(int);

    virtual const char* graphicsClassName();

    virtual const char* describeClassName(QGraphicsItem*);

 signals:
    /*!
     * The terminal emits a send for user input
     */
    void send(const TMTCMessage*);

 public slots:

    void received(const TMTCMessage*);

 private:
    Q_DISABLE_COPY(Terminal)
};
#endif
