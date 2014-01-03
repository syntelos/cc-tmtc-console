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
#include "TerminalOutput.h"

const char* TerminalOutput::TerminalOutputClassName = "TerminalOutput";

TerminalOutput::TerminalOutput(QGraphicsItem *parent)
    : GraphicsBranch(parent), text(this), border(0)
{
    this->text.setTextInteractionFlags(Qt::TextBrowserInteraction);
    this->setFontSize(12);
    this->setRows(20);
    this->setColumns(80);

    QGraphicsRectItem* border = new QGraphicsRectItem(this->text.boundingRect());
    this->setBorder(border);
}
TerminalOutput::~TerminalOutput()
{
}
int TerminalOutput::getFontSize(){

    return this->text.font().pointSize();
}
void TerminalOutput::setFontSize(int fontSize){
    if (8 <= fontSize && fontSize < 100){
        QFont monospace("Courier",fontSize,QFont::Normal);
        this->text.setFont(monospace);
    }
}
int TerminalOutput::getRows(){

    return this->rows;
}
void TerminalOutput::setRows(int rows){
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
int TerminalOutput::getColumns(){

    return this->columns;
}
void TerminalOutput::setColumns(int cols){
    if (10 <= cols){
        this->columns = cols;
        {
            QFontMetrics fm(this->text.font());
            int em = fm.width('m');

            this->text.setTextWidth(cols*em);
        }
    }
}
void TerminalOutput::received(const SystemDeviceIdentifier* sid, const TMTCMessage* m){
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
QAbstractGraphicsShapeItem* TerminalOutput::getBorder(){

    return this->border;
}
void TerminalOutput::setBorder(QAbstractGraphicsShapeItem* s){

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
const char* TerminalOutput::graphicsClassName(){

    return TerminalOutputClassName;
}
const char* TerminalOutput::describeClassName(QGraphicsItem* item){
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
