/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QtGui>
#include <QFont>
#include <QRectF>

#include "Graphics/GraphicsLeaf.h"
#include "GraphicsTerminal.h"

const char* GraphicsTerminal::GraphicsTerminalClassName = "GraphicsTerminal";

GraphicsTerminal::GraphicsTerminal(QGraphicsItem *parent)
    : GraphicsBranch(parent), output(this), input(this)
{
    QRectF output = this->output.boundingRect();
    qreal x = 0;
    qreal y = output.height()+2;
    this->input.setPos(x,y);

    QObject::connect(&this->input,SIGNAL(send(const SystemMessage*)),this,SIGNAL(send(const SystemMessage*)));
}
GraphicsTerminal::~GraphicsTerminal()
{
}
int GraphicsTerminal::getFontSize(){

    return this->output.getFontSize();
}
void GraphicsTerminal::setFontSize(int fontSize){

    this->input.setFontSize(fontSize);
    this->output.setFontSize(fontSize);
}
int GraphicsTerminal::getRows(){
    /*
     * Input is always one row
     */
    return this->output.getRows()+1;
}
void GraphicsTerminal::setRows(int rows){
    /*
     * Input is always one row
     */
    this->output.setRows(rows-1);
}
int GraphicsTerminal::getColumns(){

    return this->output.getColumns();
}
void GraphicsTerminal::setColumns(int cols){

    this->input.setColumns(cols);
    this->output.setColumns(cols);
}
const char* GraphicsTerminal::graphicsClassName(){

    return GraphicsTerminalClassName;
}
const char* GraphicsTerminal::describeClassName(QGraphicsItem* item){
    GraphicsLeaf* leaf = dynamic_cast<GraphicsLeaf*>(item);
    if (leaf){
        return leaf->graphicsClassName();
    }
    else {
        GraphicsBranch* branch = dynamic_cast<GraphicsBranch*>(item);
        if (branch){
            return branch->graphicsClassName();
        }
        else {
            return GraphicsClassNameUnknown;
        }
    }
}
void GraphicsTerminal::received(const SystemMessage* m){

    this->output.received(m);
}
