/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexTableSearch.h"

MultiplexTableSearch::MultiplexTableSearch(quintptr s, quintptr e, quintptr t)
    : start(s), end(e), end_cursor(e-MX::RecordBase), term(t), cursor(t), 
      open(true)
{
}
MultiplexTableSearch::~MultiplexTableSearch(){
}
bool MultiplexTableSearch::hasPrevious(){

    quint32 search;

    for (search = 0; open && search < MX::RecordMax; search++){

        MultiplexRecord* record = reinterpret_cast<MultiplexRecord*>(cursor);
        if (record->check()){

            return true;
        }
        else {
            cursor -= 1;
            if (cursor == term){
                open = false;
                return false;
            }
            else if (start > cursor){
                cursor = end_cursor;
            }
        }
    }
    open = false;
    return false;
}
bool MultiplexTableSearch::hasNext(){

    quint32 search;

    for (search = 0; open && search < MX::RecordMax; search++){

        MultiplexRecord* record = reinterpret_cast<MultiplexRecord*>(cursor);
        if (record->check()){

            return true;
        }
        else {
            cursor += 1;
            if (cursor == term){
                open = false;
                return false;
            }
            else if (end_cursor < cursor){
                cursor = start;
            }
        }
    }
    open = false;
    return false;
}
MultiplexRecord* MultiplexTableSearch::previous(){

    if (open && start <= cursor && end_cursor >= cursor){

        MultiplexRecord* record = reinterpret_cast<MultiplexRecord*>(cursor);
        if (record->check()){
            /*
             * The offset to the previous record is >= RecordBase
             */
            cursor -= MX::RecordBase;
            if (start > cursor){
                cursor = end_cursor;
            }
            open = (cursor != term);

            return record;
        }
        else
            return 0;
    }
    else
        return 0;
}
MultiplexRecord* MultiplexTableSearch::next(){

    if (open && start <= cursor && end_cursor >= cursor){

        MultiplexRecord* record = reinterpret_cast<MultiplexRecord*>(cursor);
        if (record->check()){
            /*
             * The offset to the next record is record.length
             */
            cursor += record->length();
            if (end_cursor < cursor){
                cursor = start;
            }
            open = (cursor != term);

            return record;
        }
        else
            return 0;
    }
    else
        return 0;
}
