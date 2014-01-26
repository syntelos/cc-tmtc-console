/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "GraphicsCanvas.h"

QScriptValue graphicsCanvasToScriptValue(QScriptEngine *engine, GraphicsCanvas* const &in){
    return engine->newQObject(in);
}

void graphicsCanvasFromScriptValue(const QScriptValue &object, GraphicsCanvas* &out){
    out = qobject_cast<GraphicsCanvas*>(object.toQObject());
}

void GraphicsCanvas::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, graphicsCanvasToScriptValue, graphicsCanvasFromScriptValue);
}

GraphicsCanvas::GraphicsCanvas(QWidget* parent)
    : QGraphicsView(new GraphicsScene(), parent)
{
    initSystemScriptable(this);

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
void GraphicsCanvas::start(){
}
void GraphicsCanvas::stop(){
}
void GraphicsCanvas::read(const SystemCatalogInput& properties, const QDomElement& node){

    QString nodeName = node.localName();

    if (nodeName == "canvas"){

    }
    else {
        qDebug() << "GraphicsCanvas.read: Unrecognized catalog read node" << nodeName;
    }
}
void GraphicsCanvas::write(SystemCatalogOutput& properties, QDomElement& node){

}
