/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QByteArray>
#include <QDateTime>

#include "MultiplexRecord.h"
#include "MultiplexRecordIterator.h"

qint64 MultiplexRecord::getTime() const {

    return time.getValue();
}
quint8 MultiplexRecord::getFieldCount() const {

    return count.getValue();
}
quint32 MultiplexRecord::getFieldLength() const {

    if (check()){

        quint32 len = 0;

        MultiplexRecordIterator<MultiplexRecord> tgt(*this);

        while (tgt.hasNext()){

            len += tgt.next().length();
        }
        return len;
    }
    else {
        return 0;
    }
}
void MultiplexRecord::init(){
    this->gs = MX::GS;
    this->rs = MX::RS;
    this->time.init(QDateTime::currentMSecsSinceEpoch());
    this->count.init(0);
}
void MultiplexRecord::init(const MultiplexRecord& copy){
    if (copy.check()){
        this->gs = MX::GS;
        this->rs = MX::RS;
        this->time.init(copy.time);
        this->count.init(copy.count);

        MultiplexRecordIterator<MultiplexRecord> src(copy);
        MultiplexRecordIterator<MultiplexRecord> tgt(*this);

        while (src.hasNext()){

            tgt.next().init(src.next());
        }
    }
    else {
        init();
    }
}
bool MultiplexRecord::check() const {
    return (MX::GS == gs && MX::RS == rs &&
            time.check() && count.check());
}
bool MultiplexRecord::zero() const {
    return (0 == gs && 0 == rs &&
            time.zero() && count.zero());
}
quint32 MultiplexRecord::length() const {

    quint32 flen = getFieldLength();

    if (0 != flen){

        return (flen + MX::RecordBase);
    }
    else if (check()){

        return MX::RecordBase;
    }
    else {
        return 0;
    }
}
/*
 */
quint8 MultiplexIndexRecord::getFieldCount() const {

    return count.getValue();
}
quint32 MultiplexIndexRecord::getFieldLength() const {

    if (check()){

        quint32 len = 0;

        MultiplexRecordIterator<MultiplexIndexRecord> tgt(*this);

        while (tgt.hasNext()){

            len += tgt.next().length();
        }
        return len;
    }
    else {
        return 0;
    }
}
void MultiplexIndexRecord::init(){
    this->gs = MX::GS;
    this->rs = MX::RS;
    this->object_size.init(MX::RecordBase);
    this->ofs_first.init(MX::RecordIndexInit);
    this->ofs_last.init(MX::RecordIndexInit);
    this->count_temporal.init(1);
    this->count_spatial.init(9);
    this->count_user.init(3600);
    this->alloc.init(MX::RecordIndexCount);
    this->count.init(0);
}
bool MultiplexIndexRecord::check() const {
    return (MX::GS == gs && MX::RS == rs &&
            object_size.check() &&
            ofs_first.check() &&
            ofs_last.check() &&
            count_temporal.check() &&
            count_spatial.check() &&
            count_user.check() &&
            alloc.check() &&
            count.check());
}
bool MultiplexIndexRecord::zero() const {
    return (0 == gs && 0 == rs &&
            object_size.zero() &&
            ofs_first.zero() &&
            ofs_last.zero() &&
            count_temporal.zero() &&
            count_spatial.zero() &&
            count_user.zero() &&
            alloc.zero() &&
            count.zero());
}
quint32 MultiplexIndexRecord::length() const {

    quint32 flen = MX::RecordIndexBase;

    const quint8 alloc = this->alloc.getValue();

    if (0 != alloc){

        flen = MX::RecordIndexBase;
        flen += (alloc * MX::FieldSizeV);
        flen += (alloc * 255);
    }
    return flen;
}
/*
 * MultiplexFieldL
 */
void MultiplexFieldL::init(const qint64 value){
    this->fs = MX::FS;
    this->setValue(value);
}
void MultiplexFieldL::init(const MultiplexFieldL& copy){
    this->fs = MX::FS;
    this->setValue(copy.getValue());
}
bool MultiplexFieldL::validate(const qint64 value) const {
    return (MX::FS == this->fs && value == getValue());
}
bool MultiplexFieldL::check() const {
    return (MX::FS == this->fs);
}
bool MultiplexFieldL::zero() const {
    return (0 == this->fs);
}
quint32 MultiplexFieldL::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeL;
    }
    else {
        return 0;
    }
}
qint64 MultiplexFieldL::getValue() const {

    Type type;

    int cc;
    for (cc = 0; cc < StorageLength; cc++){
        type.data[cc] = this->data[cc];
    }

    return type.num;
}
void MultiplexFieldL::setValue(qint64 v){

    Type type;

    type.num = v;

    int cc;
    for (cc = 0; cc < StorageLength; cc++){
        this->data[cc] = type.data[cc];
    }
}
/*
 * MultiplexFieldP
 */
void MultiplexFieldP::init(const qptrdiff value){
    this->fs = MX::FS;
    this->setValue(value);
}
void MultiplexFieldP::init(const MultiplexFieldP& copy){
    this->fs = MX::FS;
    this->setValue(copy.getValue());
}
bool MultiplexFieldP::validate(const qptrdiff value) const {
    return (MX::FS == this->fs && value == getValue());
}
bool MultiplexFieldP::check() const {
    return (MX::FS == this->fs);
}
bool MultiplexFieldP::zero() const {
    return (0 == this->fs);
}
quint32 MultiplexFieldP::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeL;
    }
    else {
        return 0;
    }
}
qptrdiff MultiplexFieldP::getValue() const {

    Type type;

    int cc;
    for (cc = 0; cc < StorageLength; cc++){
        type.data[cc] = this->data[cc];
    }

    return type.num;
}
void MultiplexFieldP::setValue(qptrdiff v){

    Type type;

    type.num = v;

    int cc;
    for (cc = 0; cc < StorageLength; cc++){
        this->data[cc] = type.data[cc];
    }
}

/*
 * MultiplexFieldI
 */
void MultiplexFieldI::init(const quint32 value){
    this->fs = MX::FS;
    this->setValue(value);
}
void MultiplexFieldI::init(const MultiplexFieldI& copy){
    this->fs = MX::FS;
    this->setValue(copy.getValue());
}
bool MultiplexFieldI::validate(const quint32 value) const {
    return (MX::FS == this->fs && value == getValue());
}
bool MultiplexFieldI::check() const {
    return (MX::FS == this->fs);
}
bool MultiplexFieldI::zero() const {
    return (0 == this->fs);
}
quint32 MultiplexFieldI::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeL;
    }
    else {
        return 0;
    }
}
quint32 MultiplexFieldI::getValue() const {

    Type type;

    int cc;
    for (cc = 0; cc < StorageLength; cc++){
        type.data[cc] = this->data[cc];
    }

    return type.num;
}
void MultiplexFieldI::setValue(quint32 v){

    Type type;

    type.num = v;

    int cc;
    for (cc = 0; cc < StorageLength; cc++){
        this->data[cc] = type.data[cc];
    }
}

/*
 * MultiplexFieldV
 */
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

        if (0 == count){

            this->alloc = DefaultAlloc;
            this->storage = 0;

            int cc = 0;
            for (; cc < alloc; cc++){
                this->value[cc] = 0;
            }
            return true;
        }
        else if (StorageLimit >= count){

            const quint8 valloc = (quint8)count;

            if (0 == (valloc & (valloc - 1))){
                /*
                 * Quota exact for count = [power of two]
                 */
                this->alloc = valloc;
            }
            else if ((count + DefaultAlloc) <= StorageLimit){
                /*
                 * Quota safety
                 */
                this->alloc = (valloc+DefaultAlloc);
            }
            else {
                /*
                 * Quota limit
                 */
                this->alloc = StorageLimit;
            }

            this->storage = valloc;

            const char* vbary = bary.data();

            quint32 cc;
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
bool MultiplexFieldV::check() const {
    return (MX::FS == this->fs);
}
bool MultiplexFieldV::zero() const {
    return (0 == this->fs);
}
quint32 MultiplexFieldV::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeV + alloc;
    }
    else {
        return 0;
    }
}
bool MultiplexFieldV::setValue(const QVariant& value){

    if (0 == this->alloc){

        return false;
    }
    /*
     * This proceedure depends on (uses) the value of 'alloc' defined
     * by 'init(QVariant)'
     */
    else if (value.isNull()){
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

/*
 * MultiplexFieldB
 */
void MultiplexFieldB::init(const quint8 value){
    this->fs = MX::FS;
    this->data = value;
}
void MultiplexFieldB::init(const MultiplexFieldB& copy){
    this->fs = MX::FS;
    this->setValue(copy.getValue());
}
bool MultiplexFieldB::validate(const quint8 value) const {

    const quint8 data = this->data;

    return (MX::FS == this->fs && value == data);
}
bool MultiplexFieldB::check() const {
    return (MX::FS == this->fs);
}
bool MultiplexFieldB::zero() const {
    return (0 == this->fs);
}
quint32 MultiplexFieldB::length() const {
    if (MX::FS == this->fs){

        return MX::FieldSizeB;
    }
    else {
        return 0;
    }
}
quint8 MultiplexFieldB::getValue() const {

    Type type;

    type.data = this->data;

    return type.num;
}
void MultiplexFieldB::setValue(quint8 v){

    Type type;

    type.num = v;

    this->data = type.data;
}
