/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDateTime>

#include "SystemMessage.h"

/*!
 */
SystemMessage::SystemMessage(const SystemDeviceIdentifier& sid)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), identifier(sid)
{
}
/*!
 */
SystemMessage::SystemMessage(const SystemDeviceIdentifier& sid, qint64 time)
    : QObject(), QList(), time(time), identifier(sid)
{
}
/*!
 * Parse "NV( NV)*" as {NV,(NV)*}
 */
SystemMessage::SystemMessage(const SystemDeviceIdentifier& sid, const QByteArray& in)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), identifier(sid)
{
    QList<QByteArray> string = in.split(' ');
    int cc, cz = string.size();
    for (cc = 0; cc < cz; cc++){
        const QByteArray& substring = string.at(cc);

        SystemNameValue* nvp = new SystemNameValue(substring);

        if (nvp->hasName())

            this->append(nvp);
        else 
            delete nvp;
    }
}
SystemMessage::SystemMessage(const SystemMessage& in)
    : QObject(), QList(), time(in.time), identifier(in.identifier)
{
    const int count = in.size();

    int cc;
    for (cc = 0; cc < count; cc++){

        this->append(new SystemNameValue(in.at(cc)));
    }
}
SystemMessage::SystemMessage(const SystemMessage& in, qint64 t)
    : QObject(), QList(), time(t), identifier(in.identifier)
{
    const int count = in.size();

    int cc;
    for (cc = 0; cc < count; cc++){

        this->append(new SystemNameValue(in.at(cc)));
    }
}
/*!
 */
SystemMessage::~SystemMessage(){
    const int count = this->size();

    int cc;
    for (cc = 0; cc < count; cc++){
        SystemNameValue* nvp = this->takeAt(0);

        delete nvp;
    }
}
/*!
 */
QByteArray* SystemMessage::createOutput() const {
    const int count = this->size();

    QByteArray* re = new QByteArray();
    int cc;
    for (cc = 0; cc < count; cc++){
        SystemNameValue* nvp = this->at(cc);

        if (0 < cc){
            re->append(' ');
        }
        re->append(*nvp);
    }
    return re;
}
qint64 SystemMessage::getTime() const {
    return this->time;
}
bool SystemMessage::isSpecial() const {
    const int count = this->size();
    
    int cc;
    for (cc = 0; cc < count; cc++){
        SystemNameValue* nvp = this->at(cc);

        if (!nvp->isSpecial()){

            return true;
        }
    }
    return (0 < count);
}
const SystemDeviceIdentifier& SystemMessage::getIdentifier() const {

    return identifier;
}
int SystemMessage::indexOf(const SystemName& n) const {
    const int count = this->size();
    
    int cc;
    for (cc = 0; cc < count; cc++){

        SystemNameValue& nv = *this->at(cc);

        if (nv == n){

            return cc;
        }
    }
    return -1;
}
bool SystemMessage::contains(const SystemName& n) const {

    return (-1 < indexOf(n));
}
QVariant SystemMessage::getValue(const SystemName& n) const {
    const int count = this->size();
    
    int cc;
    for (cc = 0; cc < count; cc++){

        SystemNameValue& nv = *this->at(cc);

        if (nv == n){

            QVariant value = QVariant(nv.getValue());
            return value;
        }
    }
    QVariant nil;
    return nil;
}
bool SystemMessage::setValue(const SystemName& n, const QVariant& v){
    const int count = this->size();
    
    int cc;
    for (cc = 0; cc < count; cc++){

        SystemNameValue& nv = *this->at(cc);

        if (nv == n){

            nv.setValue(v);
            return true;
        }
    }
    SystemNameValue* nvp = new SystemNameValue(n,v);
    append(nvp);
    return true;
}
