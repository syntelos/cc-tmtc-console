/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemMultiplexTableMem.h"


SystemMultiplexTableMem::SystemMultiplexTableMem(const SystemDeviceIdentifier& id, QObject* p)
    : SystemMultiplexTable(id,p), alloc(300)
{
}
SystemMultiplexTableMem::~SystemMultiplexTableMem()
{
    QObjectList& children = const_cast<QObjectList&>(QObject::children());
    const int count = children.size();
    int cc;
    for (cc = 0; cc < count; cc++){
        QObject* child = children.takeAt(0);
        delete child;
    }
}
quint32 SystemMultiplexTableMem::getOverheadTemporal(){
    return 0;
}
void SystemMultiplexTableMem::setOverheadTemporal(quint32){
}
quint32 SystemMultiplexTableMem::getOverheadSpatial(){
    return 0;
}
void SystemMultiplexTableMem::setOverheadSpatial(quint32){
}
quint32 SystemMultiplexTableMem::getRecordCount(){
    return alloc;
}
void SystemMultiplexTableMem::setRecordCount(quint32 z){
    if (z > alloc){
        alloc = z;
    }
}
SystemMessage* SystemMultiplexTableMem::query(const SystemMessage& q){

    QReadLocker read(&lock);

    SystemMessage* re = new SystemMessage(identifier);

    SystemMessage* last = this->last(); 
    if (last){

        foreach (const SystemNameValue& qnv, q){

            const SystemName& qn = qnv.getName();

            QVariant value = qnv.getValue();

            if (value.isValid()){

                re->append(new SystemNameValue(qn,value));
            }
        }
    }
    return re;
}
QVariant SystemMultiplexTableMem::query(const SystemName& qn){

    QReadLocker read(&lock);

    SystemMessage* last = this->last(); 
    if (last)
        return last->getValue(qn);
    else {
        QVariant nil;
        return nil;
    }
}
void SystemMultiplexTableMem::update(const SystemMessage& m){

    const int count = m.size();

    if (0 < count){
        QWriteLocker write(&lock);

        SystemMessage* rec = 0;
        SystemMessage* last = this->last();
        if (last){
            rec = new SystemMessage(*last,m.getTime());
            int cc;
            for (cc = 0; cc < count; cc++){

                SystemNameValue* nvp = m.at(cc);

                if (nvp->hasName() && nvp->hasValue()){

                    const SystemName& name = nvp->getName();

                    const QVariant& value = nvp->getValue();

                    rec->setValue(name,value);
                }
            }
        }
        else {
            rec = new SystemMessage(m);
            append(rec);
        }
    }
}
void SystemMultiplexTableMem::update(const SystemNameValue& nvp){

    if (nvp.hasName() && nvp.hasValue()){

        QWriteLocker write(&lock);

        SystemMessage* last = this->last();
        if (last){

            const SystemName& name = nvp.getName();

            const QVariant& value = nvp.getValue();

            last->setValue(name,value);
        }
    }
}
void SystemMultiplexTableMem::select(SystemMultiplexSelect& select){

    QReadLocker read(&lock);

    QList<SystemMessage*>& list = *this;

    foreach (SystemMessage* rec, list){

        select += *rec;
    }
}
void SystemMultiplexTableMem::start(){
    SystemCatalogNode::start(this);
}
void SystemMultiplexTableMem::stop(){
    SystemCatalogNode::stop(this);
}
void SystemMultiplexTableMem::read(const SystemCatalogInput& properties, const QDomElement& node){
}
void SystemMultiplexTableMem::write(SystemCatalogOutput& properties, QDomElement& node){
}
