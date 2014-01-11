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
    : id(id), index(id),
      overhead_temporal(1), overhead_spatial(9), 
      user_record_count(3600), record_count(3610), 
      file(id.toString("table")), data(0), first(0), last(0)
{
    index.read();
}
MultiplexTable::~MultiplexTable(){
}
bool MultiplexTable::isReady(){
    return (0 != record_count);
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
    if (0 == data && 0 != record_count){
        /*
         * Basic initialization of MultiplexIndex
         */
        if (index.maxObjectSize(MX::RecordBase)){

            index.write();
        }

        int object_size = index.getObjectSize();

        if (file.open(QIODevice::ReadWrite)){
            {
                const int required_size = (record_count * object_size);
                /*
                 * Expand table file size using object size
                 */
                if (required_size > file.size()){
                    file.resize(required_size);
                    file.seek(0);
                }
            }
            data = file.map(0,file.size());

            file.close();

            if (data){
                /*
                 * Scan table for 'first' and 'last'
                 */
                void* end = (data + file.size());

                void *rp, *rl = data, *rf = data;

                qint64 pt, lt = 0;


                for (rp = data; rp < end; rp += object_size){

                    MultiplexRecord* p = record(rp);

                    if (p->check()){
                        /*
                         * Found record 'p' at 'rp'
                         */
                        pt = p->getTime();

                        if (pt > lt){
                            /*
                             * In order {l,p}, including initial
                             */
                            rl = rp;
                            /*
                             * Define first, initially
                             */
                            if (data == rf){
                                rf = rp;
                            }
                        }
                        else if (0 != pt){
                            /*
                             * Out of order {l,p}
                             * 
                             * Define 'first' as 'p'
                             */
                            rf = rp;
                            /*
                             * Exit scan
                             */
                            last = rl;
                            first = rf;
                            break;
                        }
                        else {
                            /*
                             * Record 'p' has 'time' zero
                             */
                            last = rl;
                            first = rf;
                            break;
                        }
                    }
                    else {
                        /*
                         * Terminate scan at table frontier
                         */
                        last = rl;
                        first = rf;
                        break;
                    }
                }
                return true;
            }
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
        first = 0;
        last = 0;
    }
}
void MultiplexTable::reopen(){
    /*
     * Expand file map as required by record count and object size
     * 
     * A reduction of (close; open) to preserve (data, first, last)
     */
    const int object_size = index.getObjectSize();

    const int required_size = (record_count * object_size);

    if (required_size > file.size()){

        {
            unsigned char* data = (unsigned char*)this->data;
            file.unmap(data);
        }

        if (file.open(QIODevice::ReadWrite)){

            file.resize(required_size);
            file.seek(0);

            data = file.map(0,file.size());

            file.close();

            if (!data){
                data = 0;
                first = 0;
                last = 0;
            }
        }
        else {
            data = 0;
            first = 0;
            last = 0;
        }
    }
}
inline MultiplexRecord* MultiplexTable::recordFirst(){
    return reinterpret_cast<MultiplexRecord*>(first);
}
inline MultiplexRecord* MultiplexTable::recordLast(){
    return reinterpret_cast<MultiplexRecord*>(last);
}
inline MultiplexRecord* MultiplexTable::record(void* p){
    return reinterpret_cast<MultiplexRecord*>(p);
}
MultiplexRecord* MultiplexTable::recordNew(){

    MultiplexTableIterator table(data,last,file.size());
    /*
     * Increment data series
     * 
     * Validate current node in order with (any) previous node
     */
    if (table.hasNext()){
        /*
         * Call 'table.previous' before 'table.next'
         */
        MultiplexRecord* p = table.previous();
        MultiplexRecord* n = table.current();

        if (p){
            /*
             * Copy current data set
             */
            n->init(*p);
        }
        else {
            /*
             * Clear object
             */
            n->init();
        }
        return n;
    }
    else {
        /*
         * Table is closed
         */
        return 0;
    }
}

quint32 MultiplexTable::getOverheadTemporal(){

    return overhead_temporal;
}
void MultiplexTable::setOverheadTemporal(quint32 count){

    if (0 < count){

        overhead_temporal = count;

        record_count = (user_record_count + overhead_temporal + overhead_spatial);
    }
}
quint32 MultiplexTable::getOverheadSpatial(){

    return overhead_spatial;
}
void MultiplexTable::setOverheadSpatial(quint32 count){

    if (0 < count){

        overhead_spatial = count;

        record_count = (user_record_count + overhead_temporal + overhead_spatial);
    }
}
quint32 MultiplexTable::getRecordCount(){

    return user_record_count;
}
void MultiplexTable::setRecordCount(quint32 count){

    if (0 < count){

        user_record_count = count;

        record_count = (user_record_count + overhead_temporal + overhead_spatial);
    }
    else {

        user_record_count = 0;

        record_count = 0;

        close();
    }
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

    if (0 < count && 256 > count){

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

    MultiplexTableIterator table(data,last,file.size());

    while (table.hasNext()){

        MultiplexRecord *record = table.next();

        MultiplexObject object(index,*record);

        s += object;
    }
}
