/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QDateTime>
#include <QIODevice>

#include "TMTC/TMTCNameValue.h"
#include "MultiplexTable.h"
#include "MultiplexTableIterator.h"
#include "MultiplexRecordIterator.h"

MultiplexTable::MultiplexTable(const SystemDeviceIdentifier& id)
    : id(id), index(id), file(id.toString("table")), data(0)
{
    index.read();
}
MultiplexTable::~MultiplexTable(){
    if (0 != data){
        {
            unsigned char* data = (unsigned char*)this->data;
            file.unmap(data);
        }
        data = 0;
    }
}
bool MultiplexTable::isOpen(){
    return (0 != data);
}
bool MultiplexTable::open(){
    /*
     * FILE: open, resize, mem-map, then close.
     * 
     * RECORD: define data, first, and last.
     */
    if (0 == data){

        if (file.open(QIODevice::ReadWrite)){

            {
                const quint32 object_size = index.getObjectSize();
                const quint32 record_count = index.getRecordCount();

                const qint64 required_size = (record_count * object_size);
                /*
                 * Expand table file size using object size
                 */
                if (required_size > file.size()){
                    file.resize(required_size);
                    file.seek(0);
                }
            }

            data = reinterpret_cast<quintptr>(file.map(0,file.size()));

            file.close();

            return (0 != data);
        }
        else {
            qDebug().nospace() << "MultiplexTable " << file.fileName() << " failed to open file";
        }
    }
    return false;
}
void MultiplexTable::close(){
    if (0 != data){
        {
            unsigned char* data = (unsigned char*)this->data;
            file.unmap(data);
        }
        data = 0;
    }
}
void MultiplexTable::reopen(){
    /*
     * Expand file map as required by record count and object size
     * 
     * A reduction of (close; open) to preserve (data, first, last)
     */
    const quint32 object_size = index.getObjectSize();
    const quint32 record_count = index.getRecordCount();

    const qint64 required_size = (record_count * object_size);

    if (required_size > file.size()){

        {
            unsigned char* data = (unsigned char*)this->data;
            file.unmap(data);
        }

        if (file.open(QIODevice::ReadWrite)){

            file.resize(required_size);
            file.seek(0);

            data = reinterpret_cast<quintptr>(file.map(0,file.size()));

            file.close();
        }
        else {
            data = 0;
        }
    }
}
inline MultiplexRecord* MultiplexTable::recordFirst(){

    if (0 != data){

        quintptr first = data + index.getFirst();

        return reinterpret_cast<MultiplexRecord*>(first);
    }
    else
        return 0;
}
inline MultiplexRecord* MultiplexTable::recordLast(){

    if (0 != data){

        quintptr last = data + index.getLast();

        return reinterpret_cast<MultiplexRecord*>(last);
    }
    else
        return 0;
}
inline MultiplexRecord* MultiplexTable::record(quintptr p){
    return reinterpret_cast<MultiplexRecord*>(p);
}
MultiplexRecord* MultiplexTable::recordNew(){

    const quintptr cursor_start = data;
    const quintptr cursor_end = index.end(data);
    const quintptr cursor_last = index.last(data);

    MultiplexRecord* prev = reinterpret_cast<MultiplexRecord*>(cursor_last);

    if (index.top()){

        prev->init();

        index.setFirst(0);
        index.setLast(0);
        index.write();

        qDebug().nospace() << "MultiplexTable.recordNew [top] " << prev;

        return prev;
    }
    else {

        if (prev->check()){

            const quintptr cursor_first = index.first(data);

            const qptrdiff object_size = prev->length();

            const qptrdiff buffer = (object_size<<1);

            quintptr cursor = (cursor_last + prev->length());

            if ((cursor + buffer) > cursor_end){

                cursor = cursor_start;

                if (cursor == cursor_first){

                    index.setFirst(object_size);
                }
            }
            else if (cursor_first > cursor_start){

                index.setFirst((cursor+object_size)-data);
            }

            MultiplexRecord* next = reinterpret_cast<MultiplexRecord*>(cursor);

            next->init(*prev);

            index.setObjectSize(object_size);
            index.setLast(cursor-data);
            index.write();

            qDebug().nospace() << "MultiplexTable.recordNew [prev] " << next;

            return next;
        }
        else {
            qDebug().nospace() << "MultiplexTable.recordNew [bug] 0x0";
            /*
             * Unreachable
             */
            return 0;
        }
    }
}

quint32 MultiplexTable::getOverheadTemporal(){

    return index.getCountTemporal();
}
void MultiplexTable::setOverheadTemporal(quint32 count){

    index.setCountTemporal(count);
}
quint32 MultiplexTable::getOverheadSpatial(){

    return index.getCountSpatial();
}
void MultiplexTable::setOverheadSpatial(quint32 count){

    index.setCountSpatial(count);
}
quint32 MultiplexTable::getRecordCount(){

    return index.getCountUser();
}
void MultiplexTable::setRecordCount(quint32 count){

    index.setCountUser(count);
}
TMTCMessage* MultiplexTable::query(const TMTCMessage& m){

    TMTCMessage* re = new TMTCMessage();

    MultiplexRecord* r = recordLast();
    if (r){

        const int count = m.size();

        MultiplexRecordIterator object(*r);

        int cc;
        for (cc = 0; cc < count; cc++){
            TMTCNameValue* nvp = m.at(cc);
            if (nvp->hasName() && nvp->hasNotValue()){

                const TMTCName& qn = nvp->getName();

                if (this->index.contains(qn)){

                    object.field = this->index[qn];

                    QVariant value = object.getValue();

                    re->append(new TMTCNameValue(qn,value));
                }
            }
        }
    }
    return re;
}
QVariant MultiplexTable::query(const TMTCName& name){

    MultiplexRecord* r = recordLast();
    if (r){

        if (index.contains(name)){

            MultiplexRecordIterator object(*r);

            object.field = this->index[name];

            return object.getValue();
        }
        else {
            QVariant nil;
            return nil;
        }
    }
    else {
        QVariant nil;
        return nil;
    }
}
/*
 * Create a new record.  If not open, populate index.
 */
void MultiplexTable::update(const TMTCMessage& m){

    const int count = m.size();

    if (256 > count){

        MultiplexRecord* r = recordNew();
    
        if (r){
            r->time.value = m.getTime();

            MultiplexRecordIterator object(*r);

            bool index_dirty = false;

            bool objectsize_expanded = false;

            int cc;
            for (cc = 0; cc < count; cc++){

                TMTCNameValue* nvp = m.at(cc);

                if (nvp->hasName() && nvp->hasValue()){

                    const TMTCName& name = nvp->getName();

                    const QVariant& value = nvp->getValue();

                    if (index.contains(name)){

                        object.field = index[name];

                        if (!object.set(value)){

                            qDebug().nospace() << "MultiplexTable.update: Value has been ignored in (name: " << name << ", value: " << value << ")";
                        }
                    }
                    else {
                        const int field = index.count();

                        index[name] = field;

                        index_dirty = true;

                        object.field = field;

                        if (object.init(value)){

                            bool exp = index.maxObjectSize(object.length());

                            objectsize_expanded = (exp || objectsize_expanded);
                        }
                        else {

                            qDebug().nospace() << "MultiplexTable.update: Value has been ignored in (name: " << name << ", value: " << value << ")";
                        }
                    }
                }
            }

            if (index_dirty){

                index.write();
            }

            if (objectsize_expanded){

                reopen();
            }
        }
    }
}
/*
 * Modify the last record
 */
void MultiplexTable::update(const TMTCNameValue& nvp){
    if (nvp.hasName() && nvp.hasValue()){

        MultiplexRecord* r = recordLast();
        if (r){
            MultiplexRecordIterator object(*r);

            const TMTCName& name = nvp.getName();

            const QVariant& value = nvp.getValue();

            /*
             */
            if (index.contains(name)){
                object.field = index[name];

                if (!object.set(value)){

                    qDebug().nospace() << "MultiplexTable.update: Value has been ignored in (name: " << name << ", value: " << value << ")";
                }
            }
            else {
                const int field = index.count();
                index[name] = field;
                index.write();

                if (object.init(value)){

                    reopen();
                }
                else {
                    qDebug().nospace() << "MultiplexTable.update: Value has been ignored in (name: " << name << ", value: " << value << ")";
                }
            }
        }
    }
}
void MultiplexTable::select(MultiplexSelect& s){

    MultiplexTableIterator table(data,(data + index.getFirst()),file.size());

    while (table.hasNext()){

        MultiplexRecord *record = table.next();

        MultiplexObject object(index,*record);

        s += object;
    }
}
