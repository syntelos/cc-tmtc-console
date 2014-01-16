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

        QString strbuf;
        QTextStream str(&strbuf);
        str.setIntegerBase(16);

        str << "MultiplexIndex.init object_size: 0x" << getObjectSize();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init ofs_first: 0x" << getFirst();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init ofs_last: 0x" << getLast();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init count_temporal: 0x" << getCountTemporal();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init count_spatial: 0x" << getCountSpatial();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init count_user: 0x" << getCountUser();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init record_count: 0x" << getRecordCount();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init index_size: 0x" << getIndexSize();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();

        str << "MultiplexIndex.init table_size: 0x" << getTableSize();
        qDebug() << strbuf.toAscii().data();
        strbuf.clear();
    }
}
void MultiplexIndex::clearStorage(){

    storage = 0;
}
qptrdiff MultiplexIndex::getObjectSize() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->object_size.getValue();
    }
    return MX::RecordBase;
}
void MultiplexIndex::setObjectSize(qptrdiff size){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        qptrdiff object_size = storage->object_size.getValue();

        if (0 < size && size > object_size){

            storage->object_size.setValue(size);

            QString strbuf;
            QTextStream str(&strbuf);
            str.setIntegerBase(16);

            str << "MultiplexIndex.setObjectSize 0x" << size;
            qDebug() << strbuf.toAscii().data();
            strbuf.clear();

        }
    }
}
quintptr MultiplexIndex::start(quintptr start) const{

    return (start + getIndexSize());
}
quintptr MultiplexIndex::first(quintptr start) const{

    qptrdiff ofs = getFirst();
    quintptr adr = (start + ofs);

    QString strbuf;
    QTextStream str(&strbuf);
    str.setIntegerBase(16);

    str << "MultiplexIndex.first 0x" << adr << " = 0x" << start << " + 0x" << ofs;
    qDebug() << strbuf.toAscii().data();
    strbuf.clear();

    return adr;
}
qptrdiff MultiplexIndex::getFirst() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->ofs_first.getValue();
    }
    return getIndexSize();
}
void MultiplexIndex::setFirst(qptrdiff ofs){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (getIndexSize() <= ofs){

            storage->ofs_first.setValue(ofs);

            QString strbuf;
            QTextStream str(&strbuf);
            str.setIntegerBase(16);

            str << "MultiplexIndex.setFirst 0x" << ofs;
            qDebug() << strbuf.toAscii().data();
            strbuf.clear();
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

        return storage->ofs_last.getValue();
    }
    else
        return getIndexSize();
}
quintptr MultiplexIndex::last(quintptr start) const {

    qptrdiff ofs = getLast();
    quintptr adr = (start + ofs);

    QString strbuf;
    QTextStream str(&strbuf);
    str.setIntegerBase(16);

    str << "MultiplexIndex.last 0x" << adr << " = 0x" << start << " + 0x" << ofs;
    qDebug() << strbuf.toAscii().data();
    strbuf.clear();

    return adr;
}
void MultiplexIndex::setLast(qptrdiff ofs){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (getIndexSize() <= ofs){

            storage->ofs_last.setValue(ofs);

            QString strbuf;
            QTextStream str(&strbuf);
            str.setIntegerBase(16);

            str << "MultiplexIndex.setLast 0x" << ofs;
            qDebug() << strbuf.toAscii().data();
            strbuf.clear();
        }
    }
}
void MultiplexIndex::setLast(quintptr cursor, quintptr start){

    setLast( cursor - start);
}
quint32 MultiplexIndex::getCountTemporal() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);
        return storage->count_temporal.getValue();
    }
    else
        return 1;
}
void MultiplexIndex::setCountTemporal(quint32 count){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (0 != count && count > storage->count_temporal.getValue()){

            storage->count_temporal.setValue(count);

            QString strbuf;
            QTextStream str(&strbuf);
            str.setIntegerBase(16);

            str << "MultiplexIndex.setCountTemporal 0x" << count;
            qDebug() << strbuf.toAscii().data();
            strbuf.clear();
        }
    }
}
quint32 MultiplexIndex::getCountSpatial() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->count_spatial.getValue();
    }
    else
        return 9;
}
void MultiplexIndex::setCountSpatial(quint32 count){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (0 != count && count > storage->count_spatial.getValue()){

            storage->count_spatial.setValue(count);

            QString strbuf;
            QTextStream str(&strbuf);
            str.setIntegerBase(16);

            str << "MultiplexIndex.setCountSpatial 0x" << count;
            qDebug() << strbuf.toAscii().data();
            strbuf.clear();
        }
    }
}
quint32 MultiplexIndex::getCountUser() const {
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        return storage->count_user.getValue();
    }
    else
        return 3600;
}
void MultiplexIndex::setCountUser(quint32 count){
    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);

        if (0 != count && count > storage->count_user.getValue()){

            storage->count_user.setValue(count);

            QString strbuf;
            QTextStream str(&strbuf);
            str.setIntegerBase(16);

            str << "MultiplexIndex.setCountUser 0x" << count;
            qDebug() << strbuf.toAscii().data();
            strbuf.clear();
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

    QString strbuf;
    QTextStream str(&strbuf);
    str.setIntegerBase(16);

    str << "MultiplexIndex.end 0x" << adr << " = 0x" << start << " + 0x" << ofs;
    qDebug() << strbuf.toAscii().data();
    strbuf.clear();

    return adr;
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

                    qDebug() << "MultiplexIndex.index(" << n.toString() << ") [found:" << index << "]";

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

            qDebug() << "MultiplexIndex.index(" << n.toString() << ") [create:" << index << "]";

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

    if (0 != storage){
        MultiplexIndexRecord* storage = reinterpret_cast<MultiplexIndexRecord*>(this->storage);
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
