/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QKeyEvent>

#include "TerminalText.h"

const char* TerminalText::TerminalTextClassName = "TerminalText";

TerminalText::TerminalText(QGraphicsItem* parent)
    : QGraphicsTextItem(parent)
{
}
TerminalText::~TerminalText(){
}
bool TerminalText::sceneEvent(QEvent* event){

    const QEvent::Type type = event->type();

    if (QEvent::KeyPress == type || QEvent::KeyRelease == type){
        QKeyEvent* kevt = dynamic_cast<QKeyEvent*>(event);

        if (Qt::Key_Tab == kevt->key()){

            return QGraphicsItem::sceneEvent(event);
        }
        else if (QEvent::KeyPress == type){

            QGraphicsTextItem::keyPressEvent(kevt);
            return true;
        }
        else {
            QGraphicsTextItem::keyReleaseEvent(kevt);
            return true;
        }
    }
    else {
        return QGraphicsTextItem::sceneEvent(event);
    }
}
const char* TerminalText::graphicsClassName(){

    return TerminalTextClassName;
}

