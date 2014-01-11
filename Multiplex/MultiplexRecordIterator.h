/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexRecordIterator_H
#define _CONSOLE_MULTIPLEX_MultiplexRecordIterator_H

#include <QVariant>

#include "MultiplexRecord.h"

/*!
 * Low level record data arithmetic operator is used by \class
 * MultiplexTable and \class MultiplexRecord exclusively.
 * 
 * This iterator initializes to the first data field, tests the
 * current data field, and increments to the subsequent data field.
 * The 'next' operator returns the current data field, "then"
 * increments the current data field.
 * 
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
    bool init(const QVariant&);

    bool init(const MultiplexFieldV&);
    /*!
     * Call on the current field
     */
    bool set(const QVariant&);
    /*!
     * Call on the current field
     */
    QVariant getValue() const;
    /*!
     * Call on the current field
     */
    int length() const;

};
#endif
