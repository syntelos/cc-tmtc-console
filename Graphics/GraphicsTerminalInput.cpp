/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QtGui>
#include <QDebug>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QPen>
#include <QTextDocument>

#include "Graphics/GraphicsLeaf.h"
#include "GraphicsTerminalInput.h"

const char* GraphicsTerminalInput::GraphicsTerminalInputClassName = "GraphicsTerminalInput";

GraphicsTerminalInput::GraphicsTerminalInput(QGraphicsItem *parent)
    : GraphicsBranch(parent), text(this), border(0), buffer(0)
{
    this->text.setTextInteractionFlags(Qt::TextEditorInteraction);
    this->setFontSize(12);
    {
        QTextDocument* document = this->text.document();

        document->setMaximumBlockCount(1);

        document->setPlainText("");

        QObject::connect(document,SIGNAL(contentsChanged()),this,SLOT(contentsChanged()));
    }
    this->setColumns(80);

    QGraphicsRectItem* border = new QGraphicsRectItem(this->text.boundingRect());
    this->setBorder(border);
}
GraphicsTerminalInput::~GraphicsTerminalInput()
{
    if (buffer){
        delete buffer;
    }
}
int GraphicsTerminalInput::getFontSize(){

    return this->text.font().pointSize();
}
void GraphicsTerminalInput::setFontSize(int fontSize){
    if (8 <= fontSize && fontSize < 100){
        QFont monospace("Courier",fontSize,QFont::Normal);
        this->text.setFont(monospace);
    }
}
int GraphicsTerminalInput::getColumns(){

    return this->columns;
}
void GraphicsTerminalInput::setColumns(int cols){
    if (10 <= cols){
        this->columns = cols;
        {
            QFontMetrics fm(this->text.font());
            int em = fm.width('m');

            this->prepareGeometryChange();

            this->text.setTextWidth(cols*em);
        }
    }
}
QAbstractGraphicsShapeItem* GraphicsTerminalInput::getBorder(){

    return this->border;
}
void GraphicsTerminalInput::setBorder(QAbstractGraphicsShapeItem* s){

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
const char* GraphicsTerminalInput::graphicsClassName(){

    return GraphicsTerminalInputClassName;
}
const char* GraphicsTerminalInput::describeClassName(QGraphicsItem* item){
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
void GraphicsTerminalInput::contentsChanged(){

    QString plain = this->text.document()->toPlainText();
    
    QString text = plain.trimmed();

    if (text.isEmpty()){

        QString* buffer = this->buffer;
        if (buffer){
            this->buffer = 0;

            SystemMessage* m = new SystemMessage(buffer->toAscii());

            delete buffer;

            emit send(m);
        }
    }
    else {
        if (this->buffer){
            delete this->buffer;
            this->buffer = 0;
        }

        this->buffer = new QString(text);
    }
}
