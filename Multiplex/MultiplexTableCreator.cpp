/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "MultiplexTableCreator.h"

MultiplexTableCreator::MultiplexTableCreator(quintptr d, quintptr p, qptrdiff z)
    : start(d), end(d+z), record_prev(0), record_next(p)
{
    /*
     * Corner case
     */
    if (0 != record_next && (record_next + MX::RecordBase) >= end){
        /*
         * Wrap
         */
        record_next = start;
    }
}
MultiplexTableCreator::~MultiplexTableCreator()
{
}
MultiplexRecord* MultiplexTableCreator::previous(){

    return reinterpret_cast<MultiplexRecord*>(record_prev);
}
bool MultiplexTableCreator::hasNext(){

    if (0 != record_next && (record_next + MX::RecordBase) < end){

        MultiplexRecord* n = reinterpret_cast<MultiplexRecord*>(record_next);

        if (n->check()){

            qDebug().nospace() << "MultiplexTableCreator.hasNext [check]";
            return true;
        }
        else if (n->zero()){

            qDebug().nospace() << "MultiplexTableCreator.hasNext [zero]";
            return true;
        }
        else {
            qDebug().nospace() << "MultiplexTableCreator.hasNext [fail rec]";
            return false;
        }
    }
    else {
        qDebug().nospace() << "MultiplexTableCreator.hasNext [fail end]";
        return false;
    }
}
MultiplexRecord* MultiplexTableCreator::next(){

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
        else if (re->zero()){

            return re;
        }
    }

    return 0;
}
qptrdiff MultiplexTableCreator::offset(){
    if (0 != record_next)
        return (record_next - start);
    else
        return 0;
}
