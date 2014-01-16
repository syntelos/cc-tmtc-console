/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QIODevice>

#include "MultiplexIndex.h"
#include "MultiplexRecord.h"
#include "MultiplexRecordIterator.h"

MultiplexIndex::MultiplexIndex(const SystemDeviceIdentifier& id)
    : id(id), prefix(id.toString()), storage(0)
{
}
MultiplexIndex::MultiplexIndex(const SystemDeviceIdentifier& id, QFile& file)
    : id(id), prefix(id.toString()), storage(0)
{
    if (file.open(QIODevice::ReadWrite)){
        /*
         * Table discovery
         */
        init((quintptr)file.map(0,file.size()));

        file.close();
    }
}
MultiplexIndex::~MultiplexIndex()
{
    storage = 0;
}
bool MultiplexIndex::hasStorage() const {

    return (0 != storage);
}
void MultiplexIndex::init(quintptr data){

    storage = data;

    if (data){

        MultiplexIndexRecord* ir = reinterpret_cast<MultiplexIndexRecord*>(data);

        if (ir->zero()){

            ir->init();
        }
    }
}
void MultiplexIndex::clearStorage(){

    storage = 0;
}
qptrdiff MultiplexIndex::getObjectSize() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->object_size.value;
    }
    return MX::RecordBase;
}
void MultiplexIndex::setObjectSize(qptrdiff size){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        qptrdiff object_size = storage->object_size.value;

        if (0 < size && size > object_size){

            storage->object_size.value = size;
        }
    }
}
quintptr MultiplexIndex::start(quintptr start) const{

    return (start + getIndexSize());
}
quintptr MultiplexIndex::first(quintptr start) const{

    return (start + getFirst());
}
qptrdiff MultiplexIndex::getFirst() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->ofs_first.value;
    }
    return getIndexSize();
}
void MultiplexIndex::setFirst(qptrdiff ofs){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (getIndexSize() <= ofs){

            storage->ofs_first.value = ofs;
        }
    }
}
void MultiplexIndex::setFirst(quintptr cursor, quintptr start){

    setFirst( cursor - start);
}
void MultiplexIndex::setFirst(quintptr cursor, qptrdiff object_size, quintptr start){

    setFirst( (cursor + object_size), start);
}
qptrdiff MultiplexIndex::getLast() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->ofs_last.value;
    }
    else
        return getIndexSize();
}
quintptr MultiplexIndex::last(quintptr start) const {

    return (start + getLast());
}
void MultiplexIndex::setLast(qptrdiff ofs){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (getIndexSize() <= ofs){

            storage->ofs_last.value = ofs;
        }
    }
}
void MultiplexIndex::setLast(quintptr cursor, quintptr start){

    setLast( cursor - start);
}
quint32 MultiplexIndex::getCountTemporal() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);
        return storage->count_temporal.value;
    }
    else
        return 1;
}
void MultiplexIndex::setCountTemporal(quint32 count){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (0 != count && count > storage->count_temporal.value){

            storage->count_temporal.value = count;
        }
    }
}
quint32 MultiplexIndex::getCountSpatial() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->count_spatial.value;
    }
    else
        return 9;
}
void MultiplexIndex::setCountSpatial(quint32 count){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (0 != count && count > storage->count_spatial.value){

            storage->count_spatial.value = count;
        }
    }
}
quint32 MultiplexIndex::getCountUser() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->count_user.value;
    }
    else
        return 3600;
}
void MultiplexIndex::setCountUser(quint32 count){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (0 != count && count > storage->count_user.value){

            storage->count_user.value = count;
        }
    }
}
quint32 MultiplexIndex::getRecordCount() const {

    return (getCountTemporal() + getCountSpatial() + getCountUser());
}
qptrdiff MultiplexIndex::getIndexSize() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);
        return storage->length();
    }
    else
        return MX::RecordIndexInit;
}
qint64 MultiplexIndex::getTableSize() const {

    const qptrdiff index_size = getIndexSize();
    const quint32 object_size = getObjectSize();
    const quint32 record_count = getRecordCount();

    return (index_size + (object_size * record_count));
}
quintptr MultiplexIndex::end(quintptr start) const {

    return start + getTableSize();
}
int MultiplexIndex::query(const TMTCName & n) const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);
        MultiplexRecordIterator<MultiplexIndexRecord> list(*storage);

        int index = 0;

        while (list.hasNext()){

            MultiplexFieldV& fv = list.next();

            QVariant qv = fv.getValue();

            TMTCName field(qv);

            if (field == n)
                return index;
            else
                index += 1;
        }
    }
    return -1;
}
int MultiplexIndex::index(const TMTCName & n) const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        MultiplexFieldV* last = 0;

        int index = 0;
        /*
         * Query
         */
        {
            MultiplexRecordIterator<MultiplexIndexRecord> list(*storage);

            while (list.hasNext()){

                MultiplexFieldV* fv = list.current();

                QVariant qv = fv->getValue();

                TMTCName field(qv);

                if (field == n){

                    return index;
                }
                else {
                    last = fv;
                    list.next();
                    index += 1;
                }
            }
        }
        /*
         * Create new
         */
        if (storage->alloc.value > storage->count.value){

            storage->count.value += 1;

            MultiplexFieldV* next = 0;

            if (0 == last){

                next = reinterpret_cast<MultiplexFieldV*>(&storage->data[0]);
            }
            else {
                const quintptr adr_last = (quintptr)last;

                const quintptr adr_next = adr_last + last->length();

                next = reinterpret_cast<MultiplexFieldV*>(adr_next);
            }

            next->init(n);

            return index;
        }
    }
    return -1;
}
QList<TMTCName> MultiplexIndex::list() const {

    QList<TMTCName> re;

    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);
        MultiplexRecordIterator<MultiplexIndexRecord> list(*storage);

        int index = 0;

        while (list.hasNext()){

            MultiplexFieldV& fv = list.next();

            QVariant qv = fv.getValue();

            TMTCName field(qv);

            re += field;
        }
    }
    return re;
}
