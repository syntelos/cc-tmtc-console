/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MemMultiplexTable.h"


MemMultiplexTable::MemMultiplexTable(const SystemDeviceIdentifier& id, QObject* parent)
    : SystemMultiplexTable(id,p);
{
}
MemMultiplexTable::~MemMultiplexTable()
{
}
quint32 MemMultiplexTable::getOverheadTemporal(){
    return 0;
}
void MemMultiplexTable::setOverheadTemporal(quint32){
}
quint32 MemMultiplexTable::getOverheadSpatial(){
    return 0;
}
void MemMultiplexTable::setOverheadSpatial(quint32){
}
quint32 MemMultiplexTable::getRecordCount(){
    return size();
}
void MemMultiplexTable::setRecordCount(quint32){
}
SystemMessage* MemMultiplexTable::query(const SystemMessage& m){
    SystemMessage* re = SystemMessage();

    SystemMessage* last = last(); 
    if (last){

    }
    return re;
}
QVariant MemMultiplexTable::query(const SystemName&){

    QVariant nil;
    return nil;
}
void MemMultiplexTable::update(const SystemMessage& m){
}
void MemMultiplexTable::update(const SystemNameValue& nvp){
}
void MemMultiplexTable::select(SystemMultiplexSelect& select){
}

