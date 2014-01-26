/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_TERMINAL_H
#define _CONSOLE_TERMINAL_H

#include <QGraphicsItem>

#include "Graphics/GraphicsBranch.h"
#include "System/SystemDeviceIdentifier.h"
#include "System/SystemMessage.h"
#include "GraphicsTerminalOutput.h"
#include "GraphicsTerminalInput.h"

/*!
 * Combines terminal output and input into one component
 */
class GraphicsTerminal : public GraphicsBranch
{
    Q_OBJECT;

    static const char* GraphicsTerminalClassName;

    GraphicsTerminalOutput output;
    GraphicsTerminalInput input;

 public:
    GraphicsTerminal(QGraphicsItem *parent = 0);
    ~GraphicsTerminal();

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
    void send(const SystemMessage*);

 public slots:

    void received(const SystemMessage*);

 private:
    Q_DISABLE_COPY(GraphicsTerminal)
};
#endif
