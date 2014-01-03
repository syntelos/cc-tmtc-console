/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexDeviceHistory.h"

MultiplexDeviceHistory::MultiplexDeviceHistory(const SystemDeviceIdentifier& id)
    : QList(), id(id)
{
}
MultiplexDeviceHistory::~MultiplexDeviceHistory(){
}
TMTCMessage* MultiplexDeviceHistory::query(const TMTCMessage* m){
    if (isEmpty())
        return 0;
    else if (m)
        return last()->query(*m);
    else
        return 0;
}
const QVariant& MultiplexDeviceHistory::query(const TMTCName& n){
    if (isEmpty())
        return MultiplexTable::Nil;
    else
        return last()->query(n);
}
bool MultiplexDeviceHistory::update(const TMTCMessage* m){
    if (m){
        if (isEmpty()){
            MultiplexTable* table = new MultiplexTable(id);
            table->update(*m);
            append(table);
        }
        else {
            MultiplexTable* last = this->last();
            MultiplexTable* next = new MultiplexTable(*last);
            next->update(*m);
            append(next);
        }
        return true;
    }
    else {
        return false;
    }
}
void MultiplexDeviceHistory::update(const TMTCNameValue& nvp){

    if (isEmpty()){
        MultiplexTable* table = new MultiplexTable(id);
        table->update(nvp);
        append(table);
    }
    else {
        MultiplexTable* last = this->last();
        MultiplexTable* next = new MultiplexTable(*last);
        next->update(nvp);
        append(next);
    }
}
void MultiplexDeviceHistory::select(MultiplexSelect& s){
    const int count = this->size();
    int cc; 
    for (cc = 0; cc < count; cc++){

        MultiplexTable* table = this->at(cc);

        if (table && (!table->isEmpty())){

            s += *table;
        }
    }
}
