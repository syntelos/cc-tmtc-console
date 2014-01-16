/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QIODevice>

#include "System/SystemTextBuffer.h"
#include "MultiplexIndex.h"
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
MultiplexIndexRecord* MultiplexIndex::getStorage() const {

    return reinterpret_cast<MultiplexIndexRecord*>(this->storage);
}
quint32 MultiplexIndex::getObjectSize() const {

    MultiplexIndexRecord* storage = getStorage();

    if (storage)
        return storage->object_size.getValue();
    else
        return MX::RecordBase;
}
void MultiplexIndex::setObjectSize(quint32 size){

    MultiplexIndexRecord* storage = getStorage();

    if (storage){

        quint32 object_size = storage->object_size.getValue();

        if (0 < size && size > object_size){

            storage->object_size.setValue(size);
        }
    }
}
quintptr MultiplexIndex::start(quintptr start) const{

    return (start + getIndexSize());
}
quintptr MultiplexIndex::first(quintptr start) const{

    qptrdiff ofs = getFirst();
    quintptr adr = (start + ofs);

    return adr;
}
qptrdiff MultiplexIndex::getFirst() const {
    MultiplexIndexRecord* storage = getStorage();

    if (storage)
        return storage->ofs_first.getValue();
    else
        return getIndexSize();
}
void MultiplexIndex::setFirst(qptrdiff ofs){
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        if (getIndexSize() <= ofs){

            storage->ofs_first.setValue(ofs);
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
    MultiplexIndexRecord* storage = getStorage();

    if (storage)
        return storage->ofs_last.getValue();
    else
        return getIndexSize();
}
quintptr MultiplexIndex::last(quintptr start) const {

    qptrdiff ofs = getLast();
    quintptr adr = (start + ofs);

    return adr;
}
void MultiplexIndex::setLast(qptrdiff ofs){
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        if (getIndexSize() <= ofs){

            storage->ofs_last.setValue(ofs);
        }
    }
}
void MultiplexIndex::setLast(quintptr cursor, quintptr start){

    setLast( cursor - start);
}
quint32 MultiplexIndex::getCountTemporal() const {
    MultiplexIndexRecord* storage = getStorage();

    if (storage)
        return storage->count_temporal.getValue();
    else
        return 1;
}
void MultiplexIndex::setCountTemporal(quint32 count){
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        if (0 != count && count > storage->count_temporal.getValue()){

            storage->count_temporal.setValue(count);
        }
    }
}
quint32 MultiplexIndex::getCountSpatial() const {
    MultiplexIndexRecord* storage = getStorage();

    if (storage)
        return storage->count_spatial.getValue();
    else
        return 9;
}
void MultiplexIndex::setCountSpatial(quint32 count){
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        if (0 != count && count > storage->count_spatial.getValue()){

            storage->count_spatial.setValue(count);
        }
    }
}
quint32 MultiplexIndex::getCountUser() const {
    MultiplexIndexRecord* storage = getStorage();

    if (storage)
        return storage->count_user.getValue();
    else
        return 3600;
}
void MultiplexIndex::setCountUser(quint32 count){
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        if (0 != count && count > storage->count_user.getValue()){

            storage->count_user.setValue(count);
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

    qint64 ofs = getTableSize();
    quintptr adr = (start + ofs);

    return adr;
}
int MultiplexIndex::query(const TMTCName & n) const {
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        QString name = n.toString();

        MultiplexRecordIterator<MultiplexIndexRecord> list(*storage);

        int index = 0;

        while (list.hasNext()){

            MultiplexFieldV& fv = list.next();

            QVariant qv = fv.getValue();

            if (name == qv.toString())
                return index;
            else
                index += 1;
        }
    }
    return -1;
}
int MultiplexIndex::index(const TMTCName & n) const {
    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        QString name = n.toString();

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

                if (name == qv.toString()){

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
        if (storage->alloc.getValue() > storage->count.getValue()){

            storage->count.setValue(storage->count.getValue() + 1);

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

    MultiplexIndexRecord* storage = getStorage();

    if (storage){
        MultiplexRecordIterator<MultiplexIndexRecord> list(*storage);

        while (list.hasNext()){

            MultiplexFieldV& fv = list.next();

            QVariant qv = fv.getValue();

            TMTCName field(qv);

            re += field;
        }
    }
    return re;
}
void MultiplexIndex::print(){
    SystemTextBuffer strbuf;


    strbuf.hex() << "MultiplexIndex object_size: 0x" << getObjectSize();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex ofs_first: 0x" << getFirst();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex ofs_last: 0x" << getLast();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex count_temporal: 0x" << getCountTemporal();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex count_spatial: 0x" << getCountSpatial();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex count_user: 0x" << getCountUser();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex record_count: 0x" << getRecordCount();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex index_size: 0x" << getIndexSize();
    qDebug() << strbuf.data();
    strbuf.clear();

    strbuf.hex() << "MultiplexIndex table_size: 0x" << getTableSize();
    qDebug() << strbuf.data();
    strbuf.clear();


    QList<TMTCName> list = this->list();
    const int count = list.count();
    int cc;
    for (cc = 0; cc < count; cc++){

        TMTCName name = list.at(cc);

        strbuf.dec() << "MultiplexIndex [" << cc << "]: " << name.toString();
        qDebug() << strbuf.data();
        strbuf.clear();
    }
}
