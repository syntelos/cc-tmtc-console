/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_HCDB_H
#define _CONSOLE_CONFIGURATION_HCDB_H

#include <QMetaProperty>
#include <QObject>
#include <QScriptable>
#include <QSqlDatabase>
#include <QUuid>
#include <QWidget>

#include "ObjectTree/ObjectTreeNode.h"
#include "Storage/StorageInterface.h"
#include "System/SystemScriptable.h"
#include "CCDB.h"
#include "Devices.h"
#include "Libraries.h"
#include "Scripts.h"
#include "Null.h"

#define HcdbDefaultDriverType "QSQLITE"
#define HcdbLibraryTableName "HCDB_LIBRARY"
#define HcdbDeviceTableName "HCDB_DEVICE"
#define HcdbScriptTableName "HCDB_SCRIPT"

/*!
 * The Host Configuration Data Base implements the \class
 * StorageInterface over \class Libraries and \class Devices.
 */
class HCDB : public ObjectTreeNode,
    public StorageInterface,
    public SystemScriptable
{
    Q_OBJECT;
    Q_PROPERTY(Libraries* libraries READ getLibraries USER false FINAL);
    Q_PROPERTY(Devices* devices READ getDevices USER false FINAL);
    Q_PROPERTY(Scripts* scripts READ getScripts USER false FINAL);

    CCDB* ccdb;
    QSqlDatabase* db;
    Libraries* libraries;
    Devices* devices;
    Scripts* scripts;

 public:
    HCDB(CCDB* ccdb, QObject* parent = vnul);
    ~HCDB();

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
    virtual bool done(int);
    /*!
     * Indempotent plumber to bring up the configuration: returns "is
     * up".
     */
    bool init();
    /*!
     */
    bool isUp();
    /*!
     */
    bool isDown();
    /*!
     * Field
     */
    CCDB* getCCDB();
    /*!
     * Derived from CCDB
     */
    const QString* getHostUuid() const;
    /*!
     * Field
     */
    Libraries* getLibraries() const;
    /*!
     * Field
     */
    Devices* getDevices() const;
    /*!
     * Field
     */
    Scripts* getScripts() const;

 private:
    Q_DISABLE_COPY(HCDB)
};

Q_DECLARE_METATYPE(HCDB*)
#endif
