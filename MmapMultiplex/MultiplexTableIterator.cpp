/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "MultiplexTableIterator.h"
#include "MultiplexTableSearch.h"

MultiplexTableIterator::MultiplexTableIterator(quintptr d, quintptr p, qptrdiff z)
    : start(d), end(d+z), end_cursor(end-MX::RecordBase), 
      record_prev(0), record_next(0)
{
    /*
     * First node search
     * 
     * The record allocation algorithm (MultiplexTable recordNew)
     * permits the first node to be overwritten unpredictably such
     * that the iteration of existing nodes must begin with a
     * corrective search for the first node.
     */
    MultiplexTableSearch search(start,end,p);

    if (search.hasNext()){

        record_next = (quintptr)search.next();
    }
}
MultiplexTableIterator::~MultiplexTableIterator()
{
}
bool MultiplexTableIterator::hasNext(){

    if (0 != record_next && record_next < end_cursor){

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

    if (0 != record_next && record_next < end_cursor){

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
