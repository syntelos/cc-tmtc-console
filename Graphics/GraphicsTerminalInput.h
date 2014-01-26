/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_TERMINAL_INPUT_H
#define _CONSOLE_TERMINAL_INPUT_H

#include <QAbstractGraphicsShapeItem>
#include <QEvent>
#include <QFocusEvent>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <QRectF>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "Graphics/GraphicsBranch.h"
#include "System/SystemDeviceIdentifier.h"
#include "System/SystemMessage.h"
#include "GraphicsTerminalText.h"

/*!
 * GraphicsTerminal input is always one row, editable.
 */
class GraphicsTerminalInput : public GraphicsBranch
{
    Q_OBJECT;

    static const char* GraphicsTerminalInputClassName;

    GraphicsTerminalText text;
    QAbstractGraphicsShapeItem* border;
    int columns;
    QString* buffer;

 public:
    GraphicsTerminalInput(QGraphicsItem *parent = 0);
    ~GraphicsTerminalInput();

    int getFontSize();
    void setFontSize(int fontSize);

    int getColumns();
    void setColumns(int cols);

    QAbstractGraphicsShapeItem* getBorder();
    void setBorder(QAbstractGraphicsShapeItem*);

    virtual const char* graphicsClassName();

    virtual const char* describeClassName(QGraphicsItem* item);

 signals:
    void send(const SystemMessage*);

 public slots:
    void contentsChanged();

 private:
    Q_DISABLE_COPY(GraphicsTerminalInput)
};
#endif
