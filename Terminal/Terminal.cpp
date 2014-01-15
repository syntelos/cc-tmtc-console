/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QtGui>
#include <QFont>
#include <QRectF>

#include "Graphics/GraphicsLeaf.h"
#include "Terminal.h"

const char* Terminal::TerminalClassName = "Terminal";

Terminal::Terminal(QGraphicsItem *parent)
    : GraphicsBranch(parent), output(this), input(this)
{
    QRectF output = this->output.boundingRect();
    qreal x = 0;
    qreal y = output.height()+2;
    this->input.setPos(x,y);

    QObject::connect(&this->input,SIGNAL(send(const TMTCMessage*)),this,SIGNAL(send(const TMTCMessage*)));
}
Terminal::~Terminal()
{
}
int Terminal::getFontSize(){

    return this->output.getFontSize();
}
void Terminal::setFontSize(int fontSize){

    this->input.setFontSize(fontSize);
    this->output.setFontSize(fontSize);
}
int Terminal::getRows(){
    /*
     * Input is always one row
     */
    return this->output.getRows()+1;
}
void Terminal::setRows(int rows){
    /*
     * Input is always one row
     */
    this->output.setRows(rows-1);
}
int Terminal::getColumns(){

    return this->output.getColumns();
}
void Terminal::setColumns(int cols){

    this->input.setColumns(cols);
    this->output.setColumns(cols);
}
const char* Terminal::graphicsClassName(){

    return TerminalClassName;
}
const char* Terminal::describeClassName(QGraphicsItem* item){
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
void Terminal::received(const TMTCMessage* m){

    this->output.received(m);
}
