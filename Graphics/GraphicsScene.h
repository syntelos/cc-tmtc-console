/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_GRAPHICS_GraphicsScene_H
#define _CONSOLE_GRAPHICS_GraphicsScene_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>

#include "GraphicsBranch.h"
/*!
 *
 */
class GraphicsScene : public QGraphicsScene {
    Q_OBJECT;

 public:
    GraphicsScene(QObject* parent = 0);
    ~GraphicsScene();

    GraphicsScene& add(GraphicsBranch*);

    GraphicsScene& debugPrintChildren();

 private:
    Q_DISABLE_COPY(GraphicsScene);
};
#endif
