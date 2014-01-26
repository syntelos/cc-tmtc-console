/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_TERMINAL_TEXT_H
#define _CONSOLE_TERMINAL_TEXT_H

#include <QEvent>
#include <QGraphicsItem>
#include <QGraphicsTextItem>

#include "Graphics/GraphicsLeaf.h"
/*!
 *
 */
class GraphicsTerminalText : public QGraphicsTextItem,
    public GraphicsLeaf
{
    static const char* GraphicsTerminalTextClassName;

 public:
    GraphicsTerminalText(QGraphicsItem* parent = 0);
    ~GraphicsTerminalText();

    const char* graphicsClassName();

    virtual bool sceneEvent(QEvent* event);

};
#endif
