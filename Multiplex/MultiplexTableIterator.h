/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexTableIterator_H
#define _CONSOLE_MULTIPLEX_MultiplexTableIterator_H

#include "MultiplexRecord.h"

/*!
 * Iterate over existing data records in ascending (time) order.
 * 
 * This iterator initializes to the first record, tests the current
 * record, and increments to the subsequent record.  The 'next'
 * operator returns the current record, "then" increments the current
 * record.
 *
 */
class MultiplexTableIterator {

    void* start;
    void* end;
    void* record_previous;
    void* record_next;

 public:
    MultiplexTableIterator(void* d, void* p, quint64 z);
    ~MultiplexTableIterator();
    /*!
     * No cursor change, test previous node
     */
    bool hasPrevious();
    /*!
     * No cursor change, validate current node independent of previous
     * node
     */
    bool hasCurrent();
    /*!
     * No cursor change, test current node and validate 'previous' and
     * 'next' order.
     */
    bool hasNext();
    /*!
     * No cursor change, return previous node.  Initially null when
     * the current node is the "first" node.
     */
    MultiplexRecord* previous();
    /*!
     * No cursor change, return current node.
     */
    MultiplexRecord* current();
    /*!
     * Cursor change, return current node
     */
    MultiplexRecord* next();

};
#endif
