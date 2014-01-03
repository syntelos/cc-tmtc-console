/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QtGui>
#include <QDebug>
#include <QList>
#include <QTransform>

#include "GraphicsBranch.h"
#include "GraphicsLeaf.h"


const char* GraphicsBranch::GraphicsClassNameUnknown = "<Class Unknown>";
const char* GraphicsBranch::GraphicsClassNameUnknownBorder = "<Class Border>";

GraphicsBranch::GraphicsBranch(QGraphicsItem* parent)
    : QGraphicsObject(parent)
{
}
GraphicsBranch::~GraphicsBranch(){
}
QRectF GraphicsBranch::boundingRect() const {

    return this->childrenBoundingRect();
}
void GraphicsBranch::paint(QPainter* p, const QStyleOptionGraphicsItem* s, QWidget* w){
}
bool GraphicsBranch::sceneEvent(QEvent* event){
    QList<QGraphicsItem*> children = this->childItems();
    const int count = children.size();
    int cc; 
    for (cc = 0; cc < count; cc++){
        QGraphicsItem* child = children.at(cc);

        GraphicsLeaf* leaf = dynamic_cast<GraphicsLeaf*>(child);
        if (leaf){
            leaf->sceneEvent(event);
        }
        else {
            GraphicsBranch* branch = dynamic_cast<GraphicsBranch*>(child);
            if (branch){
                branch->sceneEvent(event);
            }
        }
    }
    return true;
}
