/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexIndex_H
#define _CONSOLE_MULTIPLEX_MultiplexIndex_H

#include <QHash>
#include <QVariant>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCName.h"

/*!
 * 
 */
class MultiplexIndex {

    const SystemDeviceIdentifier& id;

    QString prefix;

    QHash<TMTCName,int> table;

    qptrdiff object_size;

    qptrdiff ofs_first;

    qptrdiff ofs_last;

    quint32 count_temporal;

    quint32 count_spatial;

    quint32 count_user;

    mutable bool dirty;

    void readFirst(const QVariant&);

    void readLast(const QVariant&);

 public:
    MultiplexIndex(const SystemDeviceIdentifier& id);
    MultiplexIndex(const MultiplexIndex& copy);
    ~MultiplexIndex();

    qptrdiff getObjectSize() const;

    void setObjectSize(qptrdiff);

    bool maxObjectSize(qptrdiff);

    quintptr first(quintptr start) const;

    qptrdiff getFirst() const;

    void setFirst(qptrdiff);

    bool top() const;

    qptrdiff getLast() const;

    qptrdiff useLast() const;

    quintptr last(quintptr start) const;

    void setLast(qptrdiff);

    quint32 getCountTemporal() const;

    void setCountTemporal(quint32);

    quint32 getCountSpatial() const;

    void setCountSpatial(quint32);

    quint32 getCountUser() const;

    void setCountUser(quint32);

    quint32 getRecordCount() const;

    qint64 getTableSize() const;

    quintptr end(quintptr start) const;

    bool read();

    bool write() const;

    bool isDirty() const;

    int count() const;

    bool contains(const TMTCName& n) const;

    void insert(const TMTCName& n, int v);

    int value(const TMTCName& n) const;

};
#endif
