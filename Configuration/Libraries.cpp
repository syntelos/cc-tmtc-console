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
    : ObjectTreeNode(parent)
{
}
Libraries::~Libraries(){

    this->clear();
}
void Libraries::clear(){

    QObjectList& children = const_cast<QObjectList&>(this->children());

    const int sz = children.size();

    if (0 < sz){
        beginRemoveNode(0,(sz-1));

        int cc;
        for (cc = 0; cc < sz; cc++){

            QObject* child = children.takeAt(cc);

            if (child){

                child->deleteLater(); // (could be in a view)
            }
        }
        endRemoveNode();
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
bool Libraries::insertObjectTreeList(){
    const QObjectList& children = this->children();
    const int index = children.size();

    if (0 == index){

        beginInsertNode(index,index);

        (new Library(this));

        endInsertNode();

        return true;
    }
    else {
        Library* last = static_cast<Library*>(children.last());

        if (last->isInert()){

            return false;
        }
        else {
            beginInsertNode(index,index);

            (new Library(this));

            endInsertNode();

            return true;
        }
    }
}
bool Libraries::removeObjectTreeList(int idx){
    if (-1 < idx){
        QObjectList& children = const_cast<QObjectList&>(this->children());
        if (idx < children.size()){

            beginRemoveNode(idx,idx);

            delete children.takeAt(idx);

            endRemoveNode();

            return true;
        }
    }
    return false;
}
