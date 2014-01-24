/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QObjectList>
#include <QScriptEngine>
#include <QString>

#include "Scripts.h"
#include "Configuration.h"


QScriptValue scriptsToScriptValue(QScriptEngine *engine, Scripts* const &in){
    return engine->newQObject(in);
}

void scriptsFromScriptValue(const QScriptValue &object, Scripts* &out){
    out = qobject_cast<Scripts*>(object.toQObject());
}

void Scripts::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, scriptsToScriptValue, scriptsFromScriptValue);

    Script::InitScriptMetaType(engine);
}

Scripts::Scripts(QObject* parent)
    : SystemCatalogNode(parent)
{
}
Scripts::~Scripts(){

    this->clear();
}
void Scripts::clear(){

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
bool Scripts::drop(Script* script){
    if (script){

        const SystemScriptSymbol* linkSource = script->getLinkSource();
        if (linkSource){
            /*
             */
            script->setParent(0);
            script->deleteLater();

            return true;
        }
    }
    return false;
}
bool Scripts::deconfigure(Script* script){
    if (script){

        const SystemScriptSymbol* linkSource = script->getLinkSource();
        if (linkSource){
            /*
             */
            script->setLinkTarget(0);

            return true;
        }
    }
    return false;
}
bool Scripts::deconfigure(const char* source){

    deconfigure(find(source));
}
void Scripts::import(SystemScriptSymbol* source, SystemScriptSymbol* target){

    if (source && target && source->isNotInert() && target->isNotInert()){

        Script* script = find(*source);
        if (0 == script){

            script = new Script(this);
            script->setLinkSource(source);
            script->setLinkTarget(target);
        }
        script->importToObjectTreeNode();
    }
}
void Scripts::start(){
    SystemCatalogNode::start(this);
}
void Scripts::stop(){
    SystemCatalogNode::stop(this);
}
void Scripts::read(const SystemCatalogInput& properties, const QDomElement& parent){

    this->clear();


    // beginInsertNode(0,(count-1));

    // while (select.next()){

    //     new Script(this);
    // }

    // endInsertNode();

}
void Scripts::write(SystemCatalogOutput& properties, QDomElement& parent){

    const QObjectList& children = this->children();

    const int count = children.size();

    if (0 < count){

        int cc;
        for (cc = 0; cc < count; cc++){

            Script* child = qobject_cast<Script*>(children.at(cc));

            if (child){

                //child->write();

            }
        }
    }
}
Script* Scripts::find(const SystemScriptSymbol& source){
    const QObjectList& children = this->children();
    const int size = children.size();
    int cc;
    for (cc = 0; cc < size; cc++){

        Script* child = dynamic_cast<Script*>(children.at(cc));
        if (child){

            const SystemScriptSymbol* childSource = child->getLinkSource();
            if (childSource && childSource->operator==(source)){

                return child;
            }
        }
    }
    return 0;
}
Script* Scripts::find(const QString& source){
    const QObjectList& children = this->children();
    const int size = children.size();
    int cc;
    for (cc = 0; cc < size; cc++){

        Script* child = dynamic_cast<Script*>(children.at(cc));
        if (child){

            const SystemScriptSymbol* childSource = child->getLinkSource();
            if (childSource && childSource->operator==(source)){

                return child;
            }
        }
    }
    return 0;
}
Script* Scripts::find(const char* source){
    const QObjectList& children = this->children();
    const int size = children.size();
    int cc;
    for (cc = 0; cc < size; cc++){

        Script* child = dynamic_cast<Script*>(children.at(cc));
        if (child){

            const SystemScriptSymbol* childSource = child->getLinkSource();
            if (childSource && childSource->operator==(source)){

                return child;
            }
        }
    }
    return 0;
}
