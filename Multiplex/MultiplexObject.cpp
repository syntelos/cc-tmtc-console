/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexObject.h"

MultiplexObject::MultiplexObject(const MultiplexIndex& index, const MultiplexRecord& record)
    : index(index), record(record)
{
}
MultiplexObject::~MultiplexObject()
{
}
bool MultiplexObject::contains(const TMTCName& n) const {
    if (index.contains(n)){

        const int field = index[n];

        const MultiplexFieldV& fv = record.data[field];

        if (fv.check()){

            return (0 != fv.storage);
        }        
    }
    return false;
}
qint64 MultiplexObject::getTime() const {

    return record.time.value;
}
QVariant MultiplexObject::operator[](const TMTCName& n) const {
    if (index.contains(n)){

        const int field = index[n];

        const MultiplexFieldV& fv = record.data[field];

        if (fv.check()){

            return fv.getValue();
        }        
    }
    return QVariant();
}
