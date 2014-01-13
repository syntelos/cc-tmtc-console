/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexTableIterator_H
#define _CONSOLE_MULTIPLEX_MultiplexTableIterator_H

#include "MultiplexRecord.h"

/*!
 * Iterate over existing data records, beginning with the first
 * record, for \class MultiplexTable select operator.
 */
class MultiplexTableIterator {

    quintptr start;
    quintptr end;

    quintptr record_prev;
    quintptr record_next;


 public:
    MultiplexTableIterator(quintptr d, quintptr p, qptrdiff z);
    ~MultiplexTableIterator();
    /*!
     * No cursor change.
     */
    bool hasNext();
    /*!
     * Cursor change, return current node and then iterate.
     */
    MultiplexRecord* next();

};
#endif
