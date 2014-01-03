/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemScriptable_H
#define _SYSTEM_SystemScriptable_H

#include <QList>
#include <QMetaObject>
#include <QScriptable>

#include "SystemScriptSymbol.h"

/*!
 * Scriptable symbols are defined in the Qt Meta Object System as
 * signals prefixed with (the define) "SYSTEM_SCRIPTABLE".
 */
class SystemScriptable
    : protected QScriptable
{

 protected:
    /*!
     */
    QList<SystemScriptSymbol*> nativeSymbols;
    /*!
     */
    QList<SystemScriptSymbol*> scriptSymbols;

 public:
    /*!
     */
    SystemScriptable();
    /*!
     */
    ~SystemScriptable();
    /*!
     */
    const QList<SystemScriptSymbol*>& getSymbolsNative() const;
    /*!
     */
    const QList<SystemScriptSymbol*>& getSymbolsScript() const;
    /*!
     */
    void addSymbolScript(const QString& symbol);

 protected:
    /*!
     * Invoke dynamic reflection (introspection) to fill the native
     * (binding link source) symbol set.
     */
    void initSystemScriptable(QObject* subclass);

};

#endif
