/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QtGui>
#include <QBrush>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsRectItem>
#include <QPen>
#include <QTextDocument>

#include "Graphics/GraphicsLeaf.h"
#include "GraphicsTerminalOutput.h"

const char* GraphicsTerminalOutput::GraphicsTerminalOutputClassName = "GraphicsTerminalOutput";

GraphicsTerminalOutput::GraphicsTerminalOutput(QGraphicsItem *parent)
    : GraphicsBranch(parent), text(this), border(0)
{
    this->text.setTextInteractionFlags(Qt::TextBrowserInteraction);
    this->setFontSize(12);
    this->setRows(20);
    this->setColumns(80);

    QGraphicsRectItem* border = new QGraphicsRectItem(this->text.boundingRect());
    this->setBorder(border);
}
GraphicsTerminalOutput::~GraphicsTerminalOutput()
{
}
int GraphicsTerminalOutput::getFontSize(){

    return this->text.font().pointSize();
}
void GraphicsTerminalOutput::setFontSize(int fontSize){
    if (8 <= fontSize && fontSize < 100){
        QFont monospace("Courier",fontSize,QFont::Normal);
        this->text.setFont(monospace);
    }
}
int GraphicsTerminalOutput::getRows(){

    return this->rows;
}
void GraphicsTerminalOutput::setRows(int rows){
    if (0 < rows){
        this->rows = rows;

        QTextDocument* document = this->text.document();

        this->prepareGeometryChange();

        document->setMaximumBlockCount(rows);

        int count = 0;
        QString fill;

        for (; count < rows; count++){
            fill += "\n";
        }
        document->setPlainText(fill);
    }
}
int GraphicsTerminalOutput::getColumns(){

    return this->columns;
}
void GraphicsTerminalOutput::setColumns(int cols){
    if (10 <= cols){
        this->columns = cols;
        {
            QFontMetrics fm(this->text.font());
            int em = fm.width('m');

            this->text.setTextWidth(cols*em);
        }
    }
}
void GraphicsTerminalOutput::received(const SystemMessage* m){
    if (m){
        QByteArray* bytes = m->createOutput();
        QString text(*bytes);

        QTextCursor textCursor = this->text.textCursor(); 
        textCursor.beginEditBlock();
        textCursor.movePosition(QTextCursor::End);
        textCursor.insertBlock();
        textCursor.insertText(text);
        textCursor.endEditBlock();

        delete bytes;
    }
}
QAbstractGraphicsShapeItem* GraphicsTerminalOutput::getBorder(){

    return this->border;
}
void GraphicsTerminalOutput::setBorder(QAbstractGraphicsShapeItem* s){

    if (this->border && s != this->border){

        QAbstractGraphicsShapeItem* t = this->border;

        this->border = 0;

        t->setParentItem(0);

        delete t;
    }

    if (s){
        s->setParentItem(this);

        this->border = s;
    }
}
const char* GraphicsTerminalOutput::graphicsClassName(){

    return GraphicsTerminalOutputClassName;
}
const char* GraphicsTerminalOutput::describeClassName(QGraphicsItem* item){
    GraphicsLeaf* leaf = dynamic_cast<GraphicsLeaf*>(item);
    if (leaf){
        return leaf->graphicsClassName();
    }
    else {
        GraphicsBranch* branch = dynamic_cast<GraphicsBranch*>(item);
        if (branch){
            return branch->graphicsClassName();
        }
        else if (item == this->border){

            return GraphicsClassNameUnknownBorder;
        }
        else {
            return GraphicsClassNameUnknown;
        }
    }
}
