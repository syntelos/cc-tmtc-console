/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QMetaMethod>

#include "SystemScriptable.h"

SystemScriptable::SystemScriptable(){
}
SystemScriptable::~SystemScriptable(){

    int size, cc;

    size = nativeSymbols.size();
    for (cc = 0; cc < size; cc++){

        SystemScriptSymbol* symbol = nativeSymbols.takeAt(cc);

        delete symbol;
    }

    size = scriptSymbols.size();
    for (cc = 0; cc < size; cc++){

        SystemScriptSymbol* symbol = scriptSymbols.takeAt(cc);

        delete symbol;
    }
}
const QList<SystemScriptSymbol*>& SystemScriptable::getSymbolsNative() const {

    return this->nativeSymbols;
}
const QList<SystemScriptSymbol*>& SystemScriptable::getSymbolsScript() const {

    return this->scriptSymbols;
}
void SystemScriptable::addSymbolScript(const QString& symbol){

    this->scriptSymbols.append(new SystemScriptSymbol(symbol));
}
void SystemScriptable::initSystemScriptable(QObject* subclassThis){

    const QMetaObject* subclass = subclassThis->metaObject();

    const int size = subclass->methodCount();

    int cc;
    for (cc = subclass->methodOffset(); cc < size; cc++){

        QMetaMethod method = subclass->method(cc);

        if (QMetaMethod::Signal == method.methodType() &&
            QMetaMethod::Public == method.access())
        {
            QString className(subclass->className());
            QString methodName(method.signature());
                    
            methodName.chop(methodName.size()-methodName.indexOf('(')-1);

            this->nativeSymbols.append(new SystemScriptSymbol(className, methodName));
        }
    }
}
