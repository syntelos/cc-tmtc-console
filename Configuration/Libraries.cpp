/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QObjectList>
#include <QString>

#include "Libraries.h"


QScriptValue librariesToScriptValue(QScriptEngine *engine, Libraries* const &in){
    return engine->newQObject(in);
}

void librariesFromScriptValue(const QScriptValue &object, Libraries* &out){
    out = qobject_cast<Libraries*>(object.toQObject());
}

void Libraries::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, librariesToScriptValue, librariesFromScriptValue);

    Library::InitScriptMetaType(engine);
}

Libraries::Libraries(QObject* parent)
    : SystemCatalogNode(parent)
{
}
Libraries::~Libraries(){

    this->clear();
}
void Libraries::clear(){

    QObjectList& children = const_cast<QObjectList&>(this->children());

    const int sz = children.size();

    if (0 < sz){

        int cc;
        for (cc = 0; cc < sz; cc++){

            QObject* child = children.takeAt(cc);

            if (child){

                child->deleteLater(); // (could be in a view)
            }
        }
    }
}
void Libraries::start(){
    SystemCatalogNode::start(this);
}
void Libraries::stop(){
    SystemCatalogNode::stop(this);
}
void Libraries::read(const SystemCatalogInput& properties, const QDomElement& parent){

    this->clear();


    // beginInsertNode(0,(count-1));

    // while (select.next()){

    //     new Library(this);
    // }

    // endInsertNode();

}
void Libraries::write(SystemCatalogOutput& properties, QDomElement& parent){


    const QObjectList& children = this->children();

    const int sz = children.size();

    if (0 < sz){

        //beginStoreNode

        int cc;
        for (cc = 0; cc < sz; cc++){

            Library* child = qobject_cast<Library*>(children.at(cc));

            if (child){

                //child->write();

            }
        }

        //endStoreNode
    }
}
