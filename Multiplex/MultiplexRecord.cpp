/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QByteArray>

#include "MultiplexRecord.h"
#include "MultiplexRecordIterator.h"

inline qint64 MultiplexRecord::getTime() const {

    return time.value;
}
inline quint8 MultiplexRecord::getFieldCount() const {

    return count.value;
}
void MultiplexRecord::init(){
    this->gs = MX::GS;
    this->rs = MX::RS;
    this->time.init(0);
    this->count.init(0);
}
void MultiplexRecord::init(const MultiplexRecord& copy){
    this->gs = MX::GS;
    this->rs = MX::RS;
    this->time.init(copy.time);
    this->count.init(copy.count);

    MultiplexRecordIterator src(copy);
    MultiplexRecordIterator tgt(*this);

    while (src.hasNext()){

        tgt.next().init(src.next());
    }
}
inline bool MultiplexRecord::check() const {
    return (MX::GS == gs && MX::RS == rs &&
            time.check() && count.check());
}
int MultiplexRecord::length() const {

    if (check()){

        int len = MX::RecordBase;

        MultiplexRecordIterator tgt(*this);

        while (tgt.hasNext()){

            len += tgt.next().length();
        }
        return len;
    }
    else {
        return 0;
    }
}
/*
 * MultiplexFieldL
 */
void MultiplexFieldL::init(const qint64 value){
    this->fs = MX::FS;
    this->value = value;
}
void MultiplexFieldL::init(const MultiplexFieldL& copy){
    this->fs = MX::FS;
    this->value = copy.value;
}
inline bool MultiplexFieldL::validate(const qint64 value) const {
    return (MX::FS == this->fs && value == this->value);
}
inline bool MultiplexFieldL::check() const {
    return (MX::FS == this->fs);
}
int MultiplexFieldL::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeL;
    }
    else {
        return 0;
    }
}
/*
 * MultiplexFieldV
 */
bool MultiplexFieldV::setValue(const QVariant& value){
    /*
     * This proceedure depends on (uses) the value of 'alloc' defined
     * by 'init(QVariant)'
     */
    if (value.isNull()){
        this->storage = 0;
        int cc;
        for (cc = 0; cc < alloc; cc++){
            this->value[cc] = 0;
        }
        return true;
    }
    else {
        QByteArray bary = value.toByteArray();

        const int count = bary.count();

        if (alloc >= count){

            const char* bary_data = bary.data();

            this->storage = (quint8)count;

            int cc;
            for (cc = 0; cc < count; cc++){
                this->value[cc] = bary_data[cc];
            }
            for (; cc < alloc; cc++){
                this->value[cc] = 0;
            }
            return true;
        }
        else {
            /*
             * Ignore (not truncate) long value
             */
            return false;
        }
    }
}
QVariant MultiplexFieldV::getValue() const {
    if (0 == this->storage){
        QVariant value;
        return value;
    }
    else {
        int len = (int)storage;
        const char* data = (const char*)this->value;

        QByteArray bary(data,len);
        QVariant value(bary);
        return value;
    }
}
bool MultiplexFieldV::init(const QVariant& value){
    /*
     * This proceedure defines the value of 'alloc' required by
     * 'setValue'
     */
    this->fs = MX::FS;
    if (value.isNull()){

        this->alloc = DefaultAlloc;
        this->storage = 0;

        int cc = 0;
        for (; cc < alloc; cc++){
            this->value[cc] = 0;
        }
        return true;
    }
    else {
        const QByteArray bary = value.toByteArray();

        const quint32 count = bary.count();

        if (StorageLimit >= count){

            const quint8 valloc = (quint8)count;

            if (0 == (valloc & (valloc - 1))){
                /*
                 * Quota exact for count = [power of two]
                 */
                this->alloc = valloc;
            }
            else if ((count + 4) <= StorageLimit){
                /*
                 * Quota safety
                 */
                this->alloc = (valloc+4);
            }
            else {
                /*
                 * Quota limit
                 */
                this->alloc = StorageLimit;
            }

            this->storage = valloc;

            const char* vbary = bary.data();

            int cc;
            for (cc = 0; cc < count; cc++){
                this->value[cc] = vbary[cc];
            }
            for (; cc < alloc; cc++){
                this->value[cc] = 0;
            }
            return true;
        }
        else {
            /*
             * Ignore (not truncate) long value
             */
            return false;
        }
    }
}
void MultiplexFieldV::init(const MultiplexFieldV& copy){
    this->fs = MX::FS;
    this->alloc = copy.alloc;
    this->storage = copy.storage;
    int cc;
    for (cc = 0; cc < storage; cc++){
        this->value[cc] = copy.value[cc];
    }
    for (; cc < alloc; cc++){
        this->value[cc] = 0;
    }
}
bool MultiplexFieldV::validate(const QVariant& value) const {
    if (MX::FS == this->fs){
        if (0 == this->storage)
            return value.isNull();
        else if (value.isNull())
            return false;
        else {
            return (this->getValue() == value);
        }
    }
    else {
        return false;
    }
}
inline bool MultiplexFieldV::check() const {
    return (MX::FS == this->fs);
}
int MultiplexFieldV::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeV + alloc;
    }
    else {
        return 0;
    }
}
/*
 * MultiplexFieldB
 */
void MultiplexFieldB::init(const quint8 value){
    this->fs = MX::FS;
    this->value = value;
}
void MultiplexFieldB::init(const MultiplexFieldB& copy){
    this->fs = MX::FS;
    this->value = copy.value;
}
inline bool MultiplexFieldB::validate(const quint8 value) const {
    return (MX::FS == this->fs && value == this->value);
}
inline bool MultiplexFieldB::check() const {
    return (MX::FS == this->fs);
}
int MultiplexFieldB::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeB;
    }
    else {
        return 0;
    }
}
