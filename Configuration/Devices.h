/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Devices_H
#define _CONSOLE_CONFIGURATION_Devices_H

#include <QMetaProperty>
#include <QObject>
#include <QSqlDatabase>
#include <QUuid>
#include <QWidget>

#include "Storage/StorageList.h"
#include "Device.h"
#include "Null.h"

class Devices : public StorageList<Device*> {
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
    Devices(QSqlDatabase* db, QObject* parent = vnul);
    /*!
     */
    ~Devices();
    /*!
     */
    const QString* getHostUuid() const;
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
    virtual bool done(int write);


 private:
    Q_DISABLE_COPY(Devices)
};

Q_DECLARE_METATYPE(Devices*)
#endif
