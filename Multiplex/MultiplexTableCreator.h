/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexTableCreator_H
#define _CONSOLE_MULTIPLEX_MultiplexTableCreator_H

#include "MultiplexRecord.h"

/*!
 * Iterate over data records for \class MultiplexTable recordNew
 * operator.
 */
class MultiplexTableCreator {

    quintptr start;
    quintptr end;

    quintptr record_prev;
    quintptr record_next;


 public:
    MultiplexTableCreator(quintptr d, quintptr p, qptrdiff z);
    ~MultiplexTableCreator();
    /*!
     * To use previous and next, call previous before next.
     */
    MultiplexRecord* previous();
    /*!
     * No cursor change, basic operational validation.
     */
    bool hasNext();
    /*!
     * Cursor change, return current node and then iterate.  To use
     * previous and next, call previous before next.  To use offset
     * and next, call offset before next.
     */
    MultiplexRecord* next();
    /*!
     * To use offset and next, call offset before next.
     */
    qptrdiff offset();

};
#endif
