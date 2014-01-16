/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QDateTime>
#include <QIODevice>
#include <QReadLocker>
#include <QWriteLocker>

#include "TMTC/TMTCNameValue.h"
#include "MultiplexTable.h"
#include "MultiplexTableIterator.h"
#include "MultiplexObject.h"
#include "MultiplexRecordIterator.h"

#define IsOpen 0 != data
#define IsClosed 0 == data

MultiplexTable::MultiplexTable(const SystemDeviceIdentifier& id)
    : id(id), index(id), file(id.toString("table")), data(0)
{
}
MultiplexTable::~MultiplexTable(){

    if (IsOpen){

        file.unmap((unsigned char*)data);

        data = 0;
    }
}
void MultiplexTable::close(){

    if (IsOpen){

        file.unmap((unsigned char*)data);

        data = 0;
    }
}
bool MultiplexTable::isOpen(){
    return IsOpen;
}
bool MultiplexTable::open(){

    if (IsClosed){

        if (file.open(QIODevice::ReadWrite)){
            /*
             * Table open/create
             */
            qint64 required_size = index.getTableSize();
            if (required_size > file.size()){
                file.resize(required_size);
                file.seek(0);
            }

            data = reinterpret_cast<quintptr>(file.map(0,file.size()));

            if (IsOpen){

                index.init(data);
                /*
                 * Table redefinition
                 */
                required_size = index.getTableSize();

                if (required_size > file.size()){

                    file.unmap((unsigned char*)data);

                    data = 0;

                    index.clearStorage();

                    file.resize(required_size);

                    file.seek(0);

                    data = reinterpret_cast<quintptr>(file.map(0,file.size()));

                    if (IsOpen){
                        index.init(data);
                    }
                }
            }

            file.close();

            return IsOpen;
        }
        else {
            qDebug().nospace() << "MultiplexTable " << file.fileName() << " failed to open file";
        }
    }
    return false;
}
void MultiplexTable::reopen(){
    /*
     * Expand file map as required
     */
    const qint64 required_size = index.getTableSize();

    if (required_size > file.size()){

        if (IsOpen){

            file.unmap((unsigned char*)data);

            data = 0;
        }

        if (file.open(QIODevice::ReadWrite)){

            file.resize(required_size);
            file.seek(0);

            data = reinterpret_cast<quintptr>(file.map(0,file.size()));

            file.close();
        }
    }
}
inline MultiplexRecord* MultiplexTable::recordFirst(){

    if (IsOpen){

        quintptr first = index.first(data);

        return reinterpret_cast<MultiplexRecord*>(first);
    }
    else
        return 0;
}
inline MultiplexRecord* MultiplexTable::recordLast(){

    if (IsOpen){

        quintptr last = index.last(data);

        return reinterpret_cast<MultiplexRecord*>(last);
    }
    else
        return 0;
}
inline MultiplexRecord* MultiplexTable::record(quintptr p){
    return reinterpret_cast<MultiplexRecord*>(p);
}
MultiplexRecord* MultiplexTable::recordNew(){

    if (IsOpen){

        qDebug().nospace() << "MultiplexTable.recordNew";

        const quintptr cursor_last = index.last(data);

        MultiplexRecord* prev = reinterpret_cast<MultiplexRecord*>(cursor_last);

        if (prev->zero()){

            prev->init();

            qDebug().nospace() << "MultiplexTable.recordNew [top] " << prev;

            return prev;
        }
        else if (prev->check()){

            const quintptr cursor_start = index.start(data);

            const quintptr cursor_end = index.end(data);

            const quintptr cursor_first = index.first(data);

            const qptrdiff object_size = prev->length();

            const qptrdiff buffer = (object_size<<1);

            quintptr cursor = (cursor_last + object_size);

            if ((cursor + buffer) > cursor_end){

                cursor = cursor_start;

                if (cursor >= cursor_first){
                    /*
                     * Establish "pushing first" case on wrap of last
                     */
                    index.setFirst(cursor,object_size,data);
                }
            }
            else if (cursor_first > cursor_start){
                /*
                 * Use "pushing first" case
                 */
                index.setFirst(cursor,object_size,data);
            }

            MultiplexRecord* next = reinterpret_cast<MultiplexRecord*>(cursor);

            next->init(*prev);

            index.setObjectSize(object_size);

            index.setLast(cursor,data);

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
    else {
        qDebug().nospace() << "MultiplexTable.recordNew [table not open]";

        return 0;
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

    QReadLocker read(&lock);

    TMTCMessage* re = new TMTCMessage();

    MultiplexRecord* r = recordLast();
    if (r){

        const int count = m.size();

        MultiplexRecordIterator<MultiplexRecord> object(*r);

        int cc;
        for (cc = 0; cc < count; cc++){
            TMTCNameValue* nvp = m.at(cc);
            if (nvp->hasName() && nvp->hasNotValue()){

                const TMTCName& qn = nvp->getName();

                object.field = this->index.query(qn);

                if (-1 < object.field){

                    QVariant value = object.getValue();

                    re->append(new TMTCNameValue(qn,value));
                }
            }
        }
    }
    return re;
}
QVariant MultiplexTable::query(const TMTCName& name){

    QReadLocker read(&lock);

    MultiplexRecord* r = recordLast();
    if (r){

        MultiplexRecordIterator<MultiplexRecord> object(*r);

        object.field = this->index.query(name);

        if (-1 < object.field){

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

    if (IsOpen && 256 > count){

        QWriteLocker write(&lock);

        MultiplexRecord* r = recordNew();
    
        if (r){
            r->time.setValue(m.getTime());

            MultiplexObject object(index,*r);

            int cc;
            for (cc = 0; cc < count; cc++){

                TMTCNameValue* nvp = m.at(cc);

                if (nvp->hasName() && nvp->hasValue()){

                    const TMTCName& name = nvp->getName();

                    const QVariant& value = nvp->getValue();

                    if (!object.setValue(name,value)){
                        qDebug().nospace() << "MultiplexTable.update(TMTCMessage): record has ignored (name: " << name.toString() << ", value: " << value.toString() << ")";
                    }
                    else {
                        qDebug().nospace() << "MultiplexTable.update(TMTCMessage): record updated with (name: " << name.toString() << ", value: " << value.toString() << ")";
                    }
                }
            }

            reopen();
        }
    }
}
/*
 * Modify the last record
 */
void MultiplexTable::update(const TMTCNameValue& nvp){

    if (IsOpen && nvp.hasName() && nvp.hasValue()){

        QWriteLocker write(&lock);

        MultiplexRecord* r = recordLast();
        if (r){
            MultiplexObject object(index,*r);

            const TMTCName& name = nvp.getName();

            const QVariant& value = nvp.getValue();

            if (!object.setValue(name,value)){

                qDebug().nospace() << "MultiplexTable.update(TMTCNameValue): Value has been ignored in (name: " << name.toString() << ", value: " << value.toString() << ")";
            }

            reopen();
        }
    }
}
void MultiplexTable::select(MultiplexSelect& s){

    if (IsOpen){

        QReadLocker read(&lock);

        MultiplexTableIterator table(data,index.first(data),file.size());

        while (table.hasNext()){

            MultiplexRecord *record = table.next();

            MultiplexObject object(index,*record);

            s += object;
        }
    }
}
