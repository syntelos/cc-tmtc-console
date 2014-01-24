/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Scripts_H
#define _CONSOLE_CONFIGURATION_Scripts_H

#include <QMetaProperty>
#include <QObject>
#include <QScriptEngine>
#include <QUuid>
#include <QWidget>

#include "System/SystemCatalogNode.h"
#include "System/SystemScriptSymbol.h"
#include "Script.h"

/*!
 * 
 */
class Scripts : public SystemCatalogNode
{
    Q_OBJECT;

 public:
    static void InitScriptMetaType(QScriptEngine* engine);
    /*!
     */
    Scripts(QObject* parent = 0);
    /*!
     */
    ~Scripts();
    /*!
     */
    const QString* getHostUuid() const;
    /*!
     * Scan for child having the link \a source reference.
     */
    Script* find(const SystemScriptSymbol& source);
    /*!
     * Scan for child having the link \a source reference.
     */
    Script* find(const QString& source);
    /*!
     * Scan for child having the link \a source reference.
     */
    Script* find(const char* source);
    /*!
     * Delete the argument from memory and from the database.  If the
     * member was not in the database, the returned value will be
     * false.
     */
    bool drop(Script* script);
    /*!
     * Update this record in the database, setting the link target to
     * null so the script will not bind and evalutate.
     */
    bool deconfigure(Script* script);

    bool deconfigure(const char* source);

    void import(SystemScriptSymbol* source, SystemScriptSymbol* target);

 public slots:
    /*!
     */
    void clear();
    /*!
     */
    virtual void start();
    /*!
     */
    virtual void stop();
    /*!
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&);
    /*!
     */
    virtual void write(SystemCatalogOutput&, QDomElement&);

 private:
    Q_DISABLE_COPY(Scripts)
};

Q_DECLARE_METATYPE(Scripts*)
#endif
