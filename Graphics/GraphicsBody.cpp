/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "GraphicsBody.h"

GraphicsBody::GraphicsBody(QWidget* parent)
    : QGraphicsView(new GraphicsScene(), parent)
{
}
GraphicsBody::~GraphicsBody(){

    GraphicsScene* scene = this->scene();
    if (scene){
        delete scene;
    }
}
inline GraphicsScene* GraphicsBody::scene(){

    return static_cast<GraphicsScene*>(QGraphicsView::scene());
}
GraphicsScene& GraphicsBody::add(GraphicsBranch* item){

    return this->scene()->add(item);
}
GraphicsScene& GraphicsBody::debugPrintChildren(){

    return this->scene()->debugPrintChildren();
}
