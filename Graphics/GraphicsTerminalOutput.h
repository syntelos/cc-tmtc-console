/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_TERMINAL_OUTPUT_H
#define _CONSOLE_TERMINAL_OUTPUT_H

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsTextItem>
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
 * GraphicsTerminal output is multiple rows.
 */
class GraphicsTerminalOutput : public GraphicsBranch
{
    Q_OBJECT;

    static const char* GraphicsTerminalOutputClassName;

    GraphicsTerminalText text;
    QAbstractGraphicsShapeItem* border;
    int rows;
    int columns;

 public:
    GraphicsTerminalOutput(QGraphicsItem *parent = 0);
    ~GraphicsTerminalOutput();

    int getFontSize();
    void setFontSize(int fontSize);

    int getRows();
    void setRows(int rows);

    int getColumns();
    void setColumns(int cols);

    QAbstractGraphicsShapeItem* getBorder();
    void setBorder(QAbstractGraphicsShapeItem*);

    virtual const char* graphicsClassName();

    virtual const char* describeClassName(QGraphicsItem* item);

 public slots:

    void received(const SystemMessage* m);

 private:
    Q_DISABLE_COPY(GraphicsTerminalOutput)
};
#endif
