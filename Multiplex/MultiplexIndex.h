/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexIndex_H
#define _CONSOLE_MULTIPLEX_MultiplexIndex_H

#include <QHash>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCName.h"

/*!
 * 
 */
class MultiplexIndex {

    const SystemDeviceIdentifier& id;

    QString prefix;

    QHash<TMTCName,int> table;

    mutable int object_size;


 public:
    MultiplexIndex(const SystemDeviceIdentifier& id);
    MultiplexIndex(const MultiplexIndex& copy);
    ~MultiplexIndex();

    int getObjectSize() const;

    void setObjectSize(int);

    bool maxObjectSize(int);

    bool read();

    bool write() const;

    int count() const;

    bool contains(const TMTCName& n) const;

    int& operator[](const TMTCName& n);

    int operator[](const TMTCName& n) const;

};
#endif
