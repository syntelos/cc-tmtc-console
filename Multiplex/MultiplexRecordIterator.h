/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexRecordIterator_H
#define _CONSOLE_MULTIPLEX_MultiplexRecordIterator_H

#include <QVariant>

#include "MultiplexRecord.h"

/*!
 * Read only (random access) record data field operator.
 * 
 * \sa MultiplexObject
 */
class MultiplexRecordIterator {
    /*!
     * Index of fields in the subject record has 'count' data fields
     */
    MultiplexFieldV** index;

 public:
    MultiplexRecordIterator(const MultiplexRecord& record);
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
#endif
