/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "GraphicsCanvas.h"

GraphicsCanvas::GraphicsCanvas(QWidget* parent)
    : QGraphicsView(new GraphicsScene(), parent)
{
}
GraphicsCanvas::~GraphicsCanvas(){

    GraphicsScene* scene = this->scene();
    if (scene){
        delete scene;
    }
}
inline GraphicsScene* GraphicsCanvas::scene(){

    return static_cast<GraphicsScene*>(QGraphicsView::scene());
}
GraphicsScene& GraphicsCanvas::add(GraphicsBranch* item){

    return this->scene()->add(item);
}
GraphicsScene& GraphicsCanvas::debugPrintChildren(){

    return this->scene()->debugPrintChildren();
}
