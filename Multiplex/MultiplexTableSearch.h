/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexTableSearch_H
#define _CONSOLE_MULTIPLEX_MultiplexTableSearch_H

#include "MultiplexRecord.h"

/*!
 * Record search iterator scans for record check no more than the
 * length of the table (using one direction).  Using both directions
 * an infinite search is possible.
 */
class MultiplexTableSearch {

    const quintptr start;
    const quintptr end;
    const quintptr end_cursor;
    const quintptr term;

    quintptr cursor;

    bool open;

 public:
    MultiplexTableSearch(quintptr s, quintptr e, quintptr t);
    ~MultiplexTableSearch();
    /*!
     * Complete cursor search down address space
     */
    bool hasPrevious();
    /*!
     * Complete cursor search up address space
     */
    bool hasNext();
    /*!
     * Initiate subsequent cursor search down address space
     */
    MultiplexRecord* previous();
    /*!
     * Initiate subsequent cursor search up address space
     */
    MultiplexRecord* next();

};
#endif
