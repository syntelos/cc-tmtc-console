/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Scripts_H
#define _CONSOLE_CONFIGURATION_Scripts_H

#include <QMetaProperty>
#include <QObject>
#include <QSqlDatabase>
#include <QUuid>
#include <QWidget>

#include "Storage/StorageList.h"
#include "System/SystemScriptSymbol.h"
#include "Script.h"

/*!
 * 
 */
class Scripts : public StorageList<Script*> {
    Q_OBJECT;
    Q_PROPERTY(QString hostUuid READ getHostUuid USER false FINAL);

    QSqlDatabase* hcdb;
    /*!
     * Create table and populate with default values
     */
    void init();

 public:
    /*!
     */
    Scripts(QSqlDatabase* db, QObject* parent = 0);
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
    /*!
     * Edit node, appending a new child: return success.
     */
    virtual bool insertObjectTreeList();
    /*!
     * Edit node, removing the indexed child: return success.
     */
    virtual bool removeObjectTreeList(int idx);

 signals:
    /*!
     */
    void readSuccess();
    /*!
     */
    void readFailure();
    /*!
     */
    void writeSuccess();
    /*!
     */
    void writeFailure();

 public slots:
    /*!
     * Clear values
     */
    virtual void clear();
    /*!
     * Read table values
     */
    virtual bool read();
    /*!
     * Write table values
     */
    virtual bool write();
    /*!
     * Write for argument zero, otherwise ignore
     */
    virtual bool done(int error);

 private:
    Q_DISABLE_COPY(Scripts)
};

Q_DECLARE_METATYPE(Scripts*)
#endif
