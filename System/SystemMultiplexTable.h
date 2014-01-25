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

#include "SystemCatalogNode.h"
#include "SystemDeviceIdentifier.h"
#include "SystemMessage.h"
#include "SystemName.h"
#include "SystemNameValue.h"
#include "SystemMultiplexSelect.h"

/*!
 * This class presents an interface to a bounded time series data
 * cache.
 * 
 * The query methods only examine the most recent available record.
 * 
 * The select method operates over (no more than) the visible
 * (reachable) extent of the series defined by the number of records
 * prescribed through this interface.  Its sequence order is
 * temporally ascending: ordered from least (first) to most recent
 * (last).
 *
 * In each update, a new record accumulates the existing information
 * of a temporally previous record before importing new information.
 * 
 * SCHEMATIC MODEL
 * 
 * The table structure (as seen via this interface) includes the data
 * source (represented by a connection) and the Syntelos TM/TC common
 * values.  Generally the table structure is intended to be dynamic,
 * however an application may be structurally static.
 * 
 * MEMORY MODEL
 *
 * The total record memory may be greater than the (user) record count
 * due to implementational requirements for temporal and spatial
 * overheads.
 * 
 * Implementations should only expand (grow or increase) record
 * overhead and count figures provided by the respective (user) setter
 * methods in order to maintain a uniform table user protocol for all
 * implementations.  Naturally, this protocol has the persistence or
 * transience of the table (and its constraint parameters).
 * Implementations without overhead requirements must return value
 * zero from the overhead parameter getter methods, and ignore input
 * to the overhead parameter setter methods.
 */
class SystemMultiplexTable : public SystemCatalogNode {

 protected:

    const SystemDeviceIdentifier& identifier;

    bool condensing;

    QReadWriteLock lock;

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
     * Whether (or not) new records are created for identical data
     * sets
     */
    bool isCondensing();
    /*!
     * Whether (or not) new records are created for identical data
     * sets
     */
    void setCondensing(bool);
    /*!
     * Returns zero when unsupported by the implementation.
     */
    virtual quint32 getOverheadTemporal() = 0;
    /*!
     * Grow value, exclusively.
     */
    virtual void setOverheadTemporal(quint32) = 0;
    /*!
     * Returns zero when unsupported by the implementation.
     */
    virtual quint32 getOverheadSpatial() = 0;
    /*!
     * Grow value, exclusively.
     */
    virtual void setOverheadSpatial(quint32) = 0;
    /*!
     * Returns the last value set, or an initial default defined by
     * the implementation.
     */
    virtual quint32 getRecordCount() = 0;
    /*!
     * The external number of records in the application domain,
     * independent of overhead requirements.  Grow value, exclusively.
     */
    virtual void setRecordCount(quint32) = 0;
    /*!
     * Retrieve the most recent information
     */
    virtual SystemMessage* query(const SystemMessage& m) = 0;
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    virtual QVariant query(const SystemName&) = 0;
    /*!
     * Create a new record, accumulating existing (most recent)
     * information before overwriting with any new information.
     */
    virtual void update(const SystemMessage& m) = 0;
    /*!
     * Modify the most recent record, if one exists.  Otherwise
     * ignore.
     */
    virtual void update(const SystemNameValue& nvp) = 0;
    /*!
     * Select a time series data set.
     */
    virtual void select(SystemMultiplexSelect& select) = 0;
    /*!
     * \sa SystemCatalogNode
     */
    virtual void start() = 0;
    /*!
     * \sa SystemCatalogNode
     */
    virtual void stop() = 0;
    /*!
     * \sa SystemCatalogNode
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&) = 0;
    /*!
     * \sa SystemCatalogNode
     */
    virtual void write(SystemCatalogOutput&, QDomElement&) = 0;

 private:
    Q_DISABLE_COPY(SystemMultiplexTable);
};
#endif
