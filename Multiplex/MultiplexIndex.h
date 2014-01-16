/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexIndex_H
#define _CONSOLE_MULTIPLEX_MultiplexIndex_H

#include <Qt>
#include <QFile>
#include <QList>
#include <QVariant>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCName.h"

/*!
 * 
 */
class MultiplexIndex {

    const SystemDeviceIdentifier& id;

    QString prefix;

    quintptr storage;

    /*!
     * Disable Copy
     */
    MultiplexIndex(const MultiplexIndex& copy);

 public:
    /*!
     */
    MultiplexIndex(const SystemDeviceIdentifier& id);
    /*!
     * Unit test open existing table file
     */
    MultiplexIndex(const SystemDeviceIdentifier& id, QFile& file);
    /*!
     */
    ~MultiplexIndex();
    /*!
     */
    bool hasStorage() const;
    /*!
     */
    void init(quintptr);
    /*!
     */
    void clearStorage();
    /*!
     * The size of a (non index) record is the length of the most
     * recent record.  This value never decreases over the life of a
     * table file.
     */
    qptrdiff getObjectSize() const;

    void setObjectSize(qptrdiff);
    /*!
     * Offset to address
     */
    quintptr start(quintptr start) const;
    /*!
     * Offset to address
     */
    quintptr first(quintptr start) const;

    qptrdiff getFirst() const;

    void setFirst(qptrdiff);

    void setFirst(quintptr,quintptr);

    void setFirst(quintptr,qptrdiff,quintptr);

    qptrdiff getLast() const;
    /*!
     * Offset to address
     */
    quintptr last(quintptr start) const;

    void setLast(qptrdiff);

    void setLast(quintptr,quintptr);
    /*!
     * Temporal overhead in (non index) records
     */
    quint32 getCountTemporal() const;

    void setCountTemporal(quint32);
    /*!
     * Spatial overhead in (non index) records.  This value never
     * decreases over the life of a table file.
     */
    quint32 getCountSpatial() const;

    void setCountSpatial(quint32);
    /*!
     * User's required number of (non index) records.  This value
     * never decreases over the life of a table file.
     */
    quint32 getCountUser() const;

    void setCountUser(quint32);
    /*!
     * Allocated number of (non index) records.  This value never
     * decreases over the life of a table file.
     */
    quint32 getRecordCount() const;
    /*!
     * Allocated size of the index record.  This value never decreases
     * over the life of a table file.
     */
    qptrdiff getIndexSize() const;
    /*!
     * Required size of the table file.  This value never decreases
     * over the life of a table file.
     */
    qint64 getTableSize() const;
    /*!
     * Offset to address
     */
    quintptr end(quintptr start) const;
    /*!
     * Return negative one for not found
     */
    int query(const TMTCName& n) const;
    /*!
     * Return negative one for missing storage, or index record
     * capacity reached.
     */
    int index(const TMTCName& n) const;
    /*!
     */
    QList<TMTCName> list() const;

};
#endif
