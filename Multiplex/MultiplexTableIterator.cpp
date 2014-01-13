/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "MultiplexTableIterator.h"

MultiplexTableIterator::MultiplexTableIterator(quintptr d, quintptr p, qptrdiff z)
    : start(d), end(d+z), record_prev(0), record_next(p)
{
    if (p){
        /*
         * First node search
         * 
         * The record allocation algorithm permits the first node to
         * be overwritten unpredictably such that the iteration of
         * existing nodes must accomodate (the inexactitude) with a
         * (this) "first node search" proceedure.
         */
        const quintptr adr_start = (quintptr)start;
        const quintptr adr_end = (quintptr)end;
        const quintptr adr_next = (quintptr)p;

        quintptr adr_cursor = adr_next;

        while (true){

            if ((adr_cursor + MX::RecordBase) < adr_end){

                MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(adr_cursor);

                if (r->check()){
                    /*
                     * If 'p' was correct, fall into here and break search.
                     */
                    record_next = adr_cursor;

                    break;
                }
                else {
                    /*
                     * If 'p' was incorrect, perform corrective search.
                     */
                    adr_cursor++;

                    if (adr_cursor == adr_next){
                        /*
                         * Terminal case: table scan complete
                         * 
                         * Weird user corner case?  Table is too small
                         * to hold a record?
                         */
                        record_next = 0;

                        break;
                    }
                }
            }
            else {
                /*
                 * Wrap
                 */
                adr_cursor = adr_start;
            }
        }
    }
}
MultiplexTableIterator::~MultiplexTableIterator()
{
}
bool MultiplexTableIterator::hasNext(){

    if (0 != record_next && (record_next + MX::RecordBase) < end){

        MultiplexRecord* n = reinterpret_cast<MultiplexRecord*>(record_next);
        /*
         * Validate RECORD_NEXT
         */
        if (n->check()){

            if (record_prev){
                /*
                 * RECORD_PREV is only defined (non zero) when "record check"
                 * passes
                 */
                MultiplexRecord* p = reinterpret_cast<MultiplexRecord*>(record_prev);
                /*
                 * Halt at (last.t < first.t)
                 */
                if (p->getTime() < n->getTime()){
                    qDebug().nospace() << "MultiplexTableIterator.hasNext [p -> n]";
                    return true;
                }
                else {
                    qDebug().nospace() << "MultiplexTableIterator.hasNext [~p -> ~n]";
                    return false;
                }
            }
            else {
                qDebug().nospace() << "MultiplexTableIterator.hasNext [n]";
                return true;
            }
        }
        else {
            qDebug().nospace() << "MultiplexTableIterator.hasNext [~n.check]";
        }
    }
    else {
        qDebug().nospace() << "MultiplexTableIterator.hasNext [~record_next]";
    }
    return false;
}
MultiplexRecord* MultiplexTableIterator::next(){

    if (0 != record_next && (record_next + MX::RecordBase) < end){

        MultiplexRecord* re = reinterpret_cast<MultiplexRecord*>(record_next);

        if (re->check()){

            int object_size = re->length();
            /*
             * RECORD_PREV is only defined (non zero) when "record check"
             * passes
             */
            record_prev = record_next;
            /*
             * RECORD_NEXT is defined optimistically (speculatively)
             */
            record_next += object_size;

            if ((record_next + object_size) > end){
                /*
                 * Wrap around
                 */
                record_next = start;
            }

            return re;
        }
    }
    return 0;
}
