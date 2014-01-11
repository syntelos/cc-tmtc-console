/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexTableIterator.h"

MultiplexTableIterator::MultiplexTableIterator(void* d, void* p, quint64 z)
    : start(d), end(d+z), record_previous(0), record_next(p)
{
    const quintptr adr_start = (quintptr)d;
    const quintptr adr_next = (quintptr)p;

    quintptr adr_cursor = (quintptr)p;
    /*
     * Walk back to previous node (record_previous), if possible.
     */
    if (0 != adr_cursor){
        MultiplexRecord* rec_next = reinterpret_cast<MultiplexRecord*>(adr_cursor);
        if (rec_next->check()){

            adr_cursor -= MX::RecordBase;

            bool check_wrap_toend = true;

            while (adr_start < adr_cursor){

                MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(adr_cursor);

                if (r->check()){
                    /*
                     * If p was "first" then any previous node
                     * would be "last" and there is no previous
                     * node for this proceedure.
                     */
                    if (r->getTime() < rec_next->getTime()){
                        /*
                         * RECORD_PREVIOUS is only defined (non zero) when "record check"
                         * passes
                         */
                        record_previous = r;
                    }
                    else {
                        check_wrap_toend = false;
                    }

                    break;
                }
                else {
                    adr_cursor--;
                }
            }

            /*
             * Wrap around (to end)
             */
            if (0 == record_previous && check_wrap_toend){

                adr_cursor = (quintptr)end;

                adr_cursor -= MX::RecordBase;

                while (adr_next < adr_cursor){
                    MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(adr_cursor);

                    if (r->check()){
                        /*
                         * Test wrap validity
                         */
                        if (r->getTime() < rec_next->getTime()){
                            /*
                             * RECORD_PREVIOUS is only defined (non zero) when "record check"
                             * passes
                             */
                            record_previous = r;
                        }

                        break;
                    }
                    else {
                        adr_cursor--;
                    }
                }
            }
        }
    }
}
MultiplexTableIterator::~MultiplexTableIterator()
{
}
bool MultiplexTableIterator::hasPrevious(){
    /*
     * RECORD_PREVIOUS is only defined (non zero) when "record check"
     * passes
     */
    return (0 != record_previous);
}
bool MultiplexTableIterator::hasCurrent(){
    if (0 != record_next){

        MultiplexRecord* n = reinterpret_cast<MultiplexRecord*>(record_next);
        /*
         * Validate RECORD_NEXT
         */
        return (n->check());
    }
    else
        return false;
}
bool MultiplexTableIterator::hasNext(){
    if (0 != record_next){

        MultiplexRecord* n = reinterpret_cast<MultiplexRecord*>(record_next);
        /*
         * Validate RECORD_NEXT
         */
        if (n->check()){

            if (record_previous){
                /*
                 * RECORD_PREVIOUS is only defined (non zero) when "record check"
                 * passes
                 */
                MultiplexRecord* p = reinterpret_cast<MultiplexRecord*>(record_previous);
                /*
                 * Halt at (last < first)
                 */
                return (p->getTime() < n->getTime());
            }
            else {

                return true;
            }
        }
    }
    return false;
}
MultiplexRecord* MultiplexTableIterator::previous(){

    return reinterpret_cast<MultiplexRecord*>(record_previous);
}
MultiplexRecord* MultiplexTableIterator::current(){

    return reinterpret_cast<MultiplexRecord*>(record_next);
}
MultiplexRecord* MultiplexTableIterator::next(){

    if (0 != record_next){

        MultiplexRecord* re = reinterpret_cast<MultiplexRecord*>(record_next);

        if (re->check()){

            int object_size = re->length();
            /*
             * RECORD_PREVIOUS is only defined (non zero) when "record check"
             * passes
             */
            record_previous = record_next;
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
            /*
             * This corner case is unreachable
             * 
            else if (record_next->check() && (record_next + record_next->length()) > end){
                 *
                 * Wrap around
                 *
                record_next = start;
            }
             */

            return re;
        }
    }
    return 0;
}
