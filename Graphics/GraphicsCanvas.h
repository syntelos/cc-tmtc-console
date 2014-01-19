/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_GRAPHICS_GraphicsCanvas_H
#define _CONSOLE_GRAPHICS_GraphicsCanvas_H

#include <QGraphicsView>
#include <QWidget>

#include "GraphicsScene.h"
#include "GraphicsBranch.h"

/*!
 *
 */
class GraphicsCanvas : public QGraphicsView {
    Q_OBJECT;

 public:
    GraphicsCanvas(QWidget* parent = 0);
    ~GraphicsCanvas();

    GraphicsScene* scene();

    GraphicsScene& add(GraphicsBranch*);

    GraphicsScene& debugPrintChildren();

 private:
    Q_DISABLE_COPY(GraphicsCanvas);
};
#endif
