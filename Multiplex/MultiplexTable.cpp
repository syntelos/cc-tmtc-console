/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDateTime>

#include "TMTC/TMTCNameValue.h"
#include "MultiplexTable.h"

const QVariant MultiplexTable::Nil = QVariant();

MultiplexTable::MultiplexTable(const SystemDeviceIdentifier& id)
    : QHash(), id(id), time(QDateTime::currentMSecsSinceEpoch())
{
}
MultiplexTable::MultiplexTable(const MultiplexTable& copy)
    : QHash(copy), id(copy.id), time(copy.time)
{
}
MultiplexTable::~MultiplexTable(){
}
qint64 MultiplexTable::getTime(){

    return this->time;
}
TMTCMessage* MultiplexTable::query(const TMTCMessage& m){

    TMTCMessage* re = new TMTCMessage();

    const int count = m.size();
    int cc;
    for (cc = 0; cc < count; cc++){
        TMTCNameValue* nvp = m.at(cc);
        if (nvp->hasName() && nvp->hasNotValue()){

            const TMTCName& qn = nvp->getName();

            if (this->contains(qn)){

                QVariant* value = this->value(qn);

                TMTCName* name = new TMTCName(qn);

                re->append(new TMTCNameValue(*name,*value));
            }
        }
    }
    return re;
}
const QVariant& MultiplexTable::query(const TMTCName& qn){
    if (this->contains(qn)){

        QVariant* value = this->value(qn);

        return *value;
    }
    else {
        return Nil;
    }
}
void MultiplexTable::update(const TMTCMessage& m){

    this->time = m.getTime();

    const int count = m.size();
    int cc;
    for (cc = 0; cc < count; cc++){

        TMTCNameValue* nvp = m.at(cc);

        MultiplexTable::update(*nvp);
    }
}
void MultiplexTable::update(const TMTCNameValue& nvp){
    if (nvp.hasName() && nvp.hasValue()){
        /*
         * Copy name and value into table
         */
        TMTCName name(nvp.getName());

        QVariant* value = new QVariant(nvp.getValue());

        this->insert(name,value);
    }
}
