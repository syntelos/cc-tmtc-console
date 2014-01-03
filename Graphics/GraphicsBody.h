/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_GRAPHICS_GraphicsBody_H
#define _CONSOLE_GRAPHICS_GraphicsBody_H

#include <QGraphicsView>
#include <QWidget>

#include "GraphicsScene.h"
#include "GraphicsBranch.h"

/*!
 *
 */
class GraphicsBody : public QGraphicsView {
    Q_OBJECT;

 public:
    GraphicsBody(QWidget* parent = 0);
    ~GraphicsBody();

    GraphicsScene* scene();

    GraphicsScene& add(GraphicsBranch*);

    GraphicsScene& debugPrintChildren();

 private:
    Q_DISABLE_COPY(GraphicsBody);
};
#endif
