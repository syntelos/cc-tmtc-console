/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_SystemMultiplexTable_H
#define _CONSOLE_DATABASE_SystemMultiplexTable_H

#include <QFile>
#include <QHash>
#include <QObject>
#include <QReadWriteLock>
#include <QVariant>

#include "SystemDeviceIdentifier.h"
#include "SystemMessage.h"
#include "SystemName.h"
#include "SystemNameValue.h"
#include "SystemMultiplexSelect.h"

/*!
 * Multiplex data cache reads and writes live time series data
 * continuously.
 */
class SystemMultiplexTable : public QObject {

 protected:

    const SystemDeviceIdentifier& identifier;

 public:
    /*!
     */
    SystemMultiplexTable(const SystemDeviceIdentifier& id, QObject* parent = 0);
    /*!
     */
    ~SystemMultiplexTable();
    /*!
     */
    const SystemDeviceIdentifier& getSystemDeviceIdentifier() const;
    /*!
     */
    virtual quint32 getOverheadTemporal() = 0;
    /*!
     */
    virtual void setOverheadTemporal(quint32) = 0;
    /*!
     */
    virtual quint32 getOverheadSpatial() = 0;
    /*!
     */
    virtual void setOverheadSpatial(quint32) = 0;
    /*!
     */
    virtual quint32 getRecordCount() = 0;
    /*!
     * The external number of records in the application domain,
     * independent of overhead requirements.
     */
    virtual void setRecordCount(quint32) = 0;
    /*!
     */
    virtual SystemMessage* query(const SystemMessage& m) = 0;
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    virtual QVariant query(const SystemName&) = 0;
    /*!
     * Create a new record.  If the cache is not open, the operation
     * of the multiplex will populate the index.
     */
    virtual void update(const SystemMessage& m) = 0;
    /*!
     */
    virtual void update(const SystemNameValue& nvp) = 0;
    /*!
     */
    virtual void select(SystemMultiplexSelect& select) = 0;

 private:
    Q_DISABLE_COPY(SystemMultiplexTable);
};
#endif
