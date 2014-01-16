/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexRecordIterator_H
#define _CONSOLE_MULTIPLEX_MultiplexRecordIterator_H

#include <QVariant>

#include "MultiplexRecord.h"

/*!
 * Read only (random access) record data field operator for \class
 * MultiplexRecord and \class MultiplexIndexRecord.
 * 
 * \sa MultiplexObject
 */
template<class Record>
class MultiplexRecordIterator {
    /*!
     * Index of fields in the subject record has 'count' data fields
     */
    MultiplexFieldV** index;

 public:
    MultiplexRecordIterator(const Record& record);
    ~MultiplexRecordIterator();
    /*!
     * Number of fields in the subject record
     */
    const int count;
    /*!
     * Index to the current field is initialized to zero
     */
    int field;
    /*!
     * Test validity of 'field' (current field)
     */
    bool hasNext() const;
    /*!
     * Return the current field via 'field' and 'index', or null
     */
    MultiplexFieldV* current() const;
    /*!
     * Increment 'field' after validating current field for returning.
     * 
     * N.B.  This method will cause a memory protection fault (segment
     * violation) if the 'field' index is invalid.  Always test
     * 'hasNext' before calling 'next'.
     */
    MultiplexFieldV& next();
    /*!
     * Call on the current field
     */
    bool setValue(const QVariant&);
    /*!
     * Call on the current field
     */
    QVariant getValue() const;
    /*!
     * Call on the current field
     */
    quint8 alloc() const;

    quint8 storage() const;

    qptrdiff length() const;

};

template<class Record>
MultiplexRecordIterator<Record>::MultiplexRecordIterator(const Record& record)
    : count(record.count.value), field(0)
{
    /*
     * Pointer arithmetic for field memory layout
     */
    quintptr adr_cursor = reinterpret_cast<quintptr>(const_cast<MultiplexFieldV*>(&record.data[0]));

    index = (new MultiplexFieldV*[count]);

    int cc;
    for (cc = 0; cc < count; cc++){

        MultiplexFieldV* fv = reinterpret_cast<MultiplexFieldV*>(adr_cursor);

        index[cc] = fv;

        adr_cursor += (MX::FieldSizeV + fv->alloc);
    }
}
template<class Record>
MultiplexRecordIterator<Record>::~MultiplexRecordIterator(){

    delete index;
}
template<class Record>
bool MultiplexRecordIterator<Record>::hasNext() const {

    return (-1 < field && field < count);
}
template<class Record>
MultiplexFieldV* MultiplexRecordIterator<Record>::current() const {

    if (-1 < field && field < count){

        return index[field];
    }
    else
        return 0;
}
template<class Record>
MultiplexFieldV& MultiplexRecordIterator<Record>::next(){

    MultiplexFieldV* current = this->current();

    field += 1;

    return *current;
}
template<class Record>
bool MultiplexRecordIterator<Record>::setValue(const QVariant& value){
    MultiplexFieldV* current = this->current();
    if (current)
        return current->setValue(value);
    else
        return false;
}
template<class Record>
QVariant MultiplexRecordIterator<Record>::getValue() const {
    MultiplexFieldV* current = this->current();
    if (current)
        return current->getValue();
    else 
        return QVariant();
}
template<class Record>
quint8 MultiplexRecordIterator<Record>::alloc() const {
    MultiplexFieldV* current = this->current();
    if (current)
        return current->alloc;
    else 
        return 0;
}
template<class Record>
quint8 MultiplexRecordIterator<Record>::storage() const {
    MultiplexFieldV* current = this->current();
    if (current)
        return current->storage;
    else 
        return 0;
}
template<class Record>
qptrdiff MultiplexRecordIterator<Record>::length() const {
    MultiplexFieldV* current = this->current();
    if (current)
        return current->length();
    else 
        return 0;
}

#endif
