/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexRecordIterator.h"

MultiplexRecordIterator::MultiplexRecordIterator(const MultiplexRecord& record)
    : count(record.count.value), field(0)
{
    void* p = const_cast<MultiplexFieldV*>(&record.data[0]);

    index = (new MultiplexFieldV*[count]);

    int cc;
    for (cc = 0; cc < count; cc++){

        MultiplexFieldV* fv = reinterpret_cast<MultiplexFieldV*>(p);

        index[cc] = fv;

        p += (MX::FieldSizeV + fv->alloc);
    }
}
MultiplexRecordIterator::~MultiplexRecordIterator(){

    delete index;
}
bool MultiplexRecordIterator::hasNext() const {

    return (-1 < field && field < count);
}
MultiplexFieldV* MultiplexRecordIterator::current() const {

    if (-1 < field && field < count){

        return index[field];
    }
    else
        return 0;
}
MultiplexFieldV& MultiplexRecordIterator::next(){

    MultiplexFieldV* current = this->current();

    field += 1;

    return *current;
}
bool MultiplexRecordIterator::init(const QVariant& value){
    MultiplexFieldV* current = this->current();
    if (current)
        return current->init(value);
    else
        return false;
}
bool MultiplexRecordIterator::init(const MultiplexFieldV& copy){
    MultiplexFieldV* current = this->current();
    if (current){
        current->init(copy);
        return true;
    }
    else
        return false;
}
bool MultiplexRecordIterator::set(const QVariant& value){
    MultiplexFieldV* current = this->current();
    if (current)
        return current->setValue(value);
    else
        return false;
}
QVariant MultiplexRecordIterator::getValue() const {
    MultiplexFieldV* current = this->current();
    if (current)
        return current->getValue();
    else 
        return QVariant();
}
int MultiplexRecordIterator::length() const {
    MultiplexFieldV* current = this->current();
    if (current)
        return current->length();
    else 
        return 0;
}
