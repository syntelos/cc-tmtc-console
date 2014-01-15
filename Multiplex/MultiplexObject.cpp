/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexObject.h"

MultiplexObject::MultiplexObject(MultiplexIndex& index, MultiplexRecord& record)
    : fcount(0), findex(0), mindex(index), mrecord(record)
{
    reindex();
}
MultiplexObject::~MultiplexObject()
{
    if (findex){
        delete findex;
        findex = 0;
    }
}
void MultiplexObject::reindex(){
    if (findex){
        delete findex;
        findex = 0;
    }
    fcount = mrecord.getFieldCount();
    if (0 < fcount){
        /*
         * Pointer arithmetic for field memory layout
         */
        quintptr adr_cursor = reinterpret_cast<quintptr>(const_cast<MultiplexFieldV*>(&mrecord.data[0]));

        findex = (new MultiplexFieldV*[fcount]);

        int cc;
        for (cc = 0; cc < fcount; cc++){

            MultiplexFieldV* fv = reinterpret_cast<MultiplexFieldV*>(adr_cursor);

            findex[cc] = fv;

            adr_cursor += (MX::FieldSizeV + fv->alloc);
        }
    }
}
qint64 MultiplexObject::getTime() const {

    return mrecord.time.value;
}
bool MultiplexObject::contains(const TMTCName& n) const {
    if (mindex.contains(n)){

        const int field = mindex.value(n);

        MultiplexFieldV* fv = findex[field];

        if (fv->check()){

            return (0 != fv->storage);
        }        
    }
    return false;
}
QVariant MultiplexObject::getValue(const TMTCName& n) const {
    if (mindex.contains(n)){

        const int field = mindex.value(n);

        MultiplexFieldV* fv = findex[field];

        if (fv->check()){

            return fv->getValue();
        }        
    }
    return QVariant();
}
bool MultiplexObject::setValue(const TMTCName& n, const QVariant& v){
    if (mindex.contains(n)){

        const int field = mindex.value(n);

        MultiplexFieldV* fv = findex[field];

        return fv->setValue(v);
    }
    else {
        const int field = mindex.count();

        mindex.insert(n,field);

        mrecord.count.value += 1;

        reindex();

        MultiplexFieldV* fv = findex[field];

        bool re = fv->init(v);

        mindex.maxObjectSize(mrecord.length());

        return re;
    }
}
quint8 MultiplexObject::alloc(const TMTCName& n) const {
    if (mindex.contains(n)){

        const int field = mindex.value(n);

        MultiplexFieldV* fv = findex[field];

        return fv->alloc;
    }
    else 
        return 0;
}
quint8 MultiplexObject::storage(const TMTCName& n) const {
    if (mindex.contains(n)){

        const int field = mindex.value(n);

        MultiplexFieldV* fv = findex[field];

        return fv->storage;
    }
    else 
        return 0;
}
qptrdiff MultiplexObject::length(const TMTCName& n) const {
    if (mindex.contains(n)){

        const int field = mindex.value(n);

        MultiplexFieldV* fv = findex[field];

        return fv->length();
    }
    else 
        return 0;
}
