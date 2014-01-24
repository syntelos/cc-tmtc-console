/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexObject_H
#define _CONSOLE_MULTIPLEX_MultiplexObject_H

#include "System/SystemName.h"
#include "MultiplexIndex.h"
#include "MultiplexRecord.h"
/*!
 * Read/write record data field operator.
 *
 * \sa MultiplexRecordIterator
 */
class MultiplexObject {

    int fcount;

    MultiplexFieldV** findex;

    MultiplexIndex& mindex;

    MultiplexRecord& mrecord;

 public:
    /*!
     */
    MultiplexObject(MultiplexIndex& index, MultiplexRecord& record);
    /*!
     */
    ~MultiplexObject();
    /*!
     */
    void reindex();
    /*!
     */
    quint8 getFieldCount();
    /*!
     */
    qint64 getTime();
    /*!
     */
    bool contains(const SystemName& n) const;
    /*!
     */
    QVariant getValue(const SystemName& n) const;
    /*!
     */
    bool setValue(const SystemName& n, const QVariant& V);
    /*!
     */
    quint8 alloc(const SystemName& n) const;
    /*!
     */
    quint8 storage(const SystemName& n) const;
    /*!
     */
    qptrdiff length(const SystemName& n) const;

};
#endif
