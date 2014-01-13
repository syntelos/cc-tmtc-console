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

    void readFirst(const QVariant&);

    void readLast(const QVariant&);

 public:
    MultiplexIndex(const SystemDeviceIdentifier& id);
    MultiplexIndex(const MultiplexIndex& copy);
    ~MultiplexIndex();

    qptrdiff getObjectSize() const;

    void setObjectSize(qptrdiff);

    bool maxObjectSize(qptrdiff);

    qptrdiff getFirst() const;

    void setFirst(qptrdiff);

    qptrdiff getLast() const;

    qptrdiff useLast() const;

    void setLast(qptrdiff);

    quint32 getCountTemporal() const;

    void setCountTemporal(quint32);

    quint32 getCountSpatial() const;

    void setCountSpatial(quint32);

    quint32 getCountUser() const;

    void setCountUser(quint32);

    quint32 getRecordCount() const;

    bool read();

    bool write() const;

    int count() const;

    bool contains(const TMTCName& n) const;

    int& operator[](const TMTCName& n);

    int operator[](const TMTCName& n) const;

};
#endif
