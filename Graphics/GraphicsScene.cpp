/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QList>

#include "GraphicsScene.h"

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
{
    this->setItemIndexMethod(QGraphicsScene::NoIndex);
}
GraphicsScene::~GraphicsScene(){
}
GraphicsScene& GraphicsScene::add(GraphicsBranch* item){

    this->addItem(item);

    return *this;
}
GraphicsScene& GraphicsScene::debugPrintChildren(){
    QList<QGraphicsItem*> sceneList = this->items();
    const int count = sceneList.size();

    GraphicsBranch* debugger = 0;

    int cc;
    for (cc = 0; cc < count; cc++){
        QGraphicsItem* sceneItem = sceneList.at(cc);

        if (!debugger){

            debugger = dynamic_cast<GraphicsBranch*>(sceneItem);
        }

        if (debugger){

            const char* sceneDesc = debugger->describeClassName(sceneItem);

            qDebug().nospace() << "QGraphicsScene [" << cc << "] " << sceneDesc;
        }
    }
    return *this;
}
