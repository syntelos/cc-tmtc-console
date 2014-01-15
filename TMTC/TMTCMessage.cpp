/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDateTime>

#include "TMTCMessage.h"

/*!
 */
TMTCMessage::TMTCMessage()
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), sid(0)
{
}
/*!
 */
TMTCMessage::TMTCMessage(SystemDeviceIdentifier* sid)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), sid(sid)
{
}
/*!
 */
TMTCMessage::TMTCMessage(qint64 time)
    : QObject(), QList(), time(time), sid(0)
{
}
/*!
 */
TMTCMessage::TMTCMessage(SystemDeviceIdentifier* sid, qint64 time)
    : QObject(), QList(), time(time), sid(sid)
{
}
/*!
 * Parse "NV( NV)*" as {NV,(NV)*}
 */
TMTCMessage::TMTCMessage(const QByteArray& in)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), sid(0)
{
    QList<QByteArray> string = in.split(' ');
    int cc, cz = string.size();
    for (cc = 0; cc < cz; cc++){
        const QByteArray& substring = string.at(cc);

        TMTCNameValue* nvp = new TMTCNameValue(substring);

        if (nvp->hasName())

            this->append(nvp);
        else 
            delete nvp;
    }
}
/*!
 * Parse "NV( NV)*" as {NV,(NV)*}
 */
TMTCMessage::TMTCMessage(SystemDeviceIdentifier* sid, const QByteArray& in)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), sid(sid)
{
    QList<QByteArray> string = in.split(' ');
    int cc, cz = string.size();
    for (cc = 0; cc < cz; cc++){
        const QByteArray& substring = string.at(cc);

        TMTCNameValue* nvp = new TMTCNameValue(substring);

        if (nvp->hasName())

            this->append(nvp);
        else 
            delete nvp;
    }
}
TMTCMessage::TMTCMessage(const TMTCMessage& in)
    : QObject(), QList(), time(in.time), sid(in.sid)
{
    const int count = in.size();

    int cc;
    for (cc = 0; cc < count; cc++){

        this->append(new TMTCNameValue(in.at(cc)));
    }
}
/*!
 */
TMTCMessage::~TMTCMessage(){
    const int count = this->size();

    int cc;
    for (cc = 0; cc < count; cc++){
        TMTCNameValue* nvp = this->takeAt(0);

        delete nvp;
    }

    sid = 0;
}
/*!
 */
QByteArray* TMTCMessage::createOutput() const {
    const int count = this->size();

    QByteArray* re = new QByteArray();
    int cc;
    for (cc = 0; cc < count; cc++){
        TMTCNameValue* nvp = this->at(cc);

        if (0 < cc){
            re->append(' ');
        }
        re->append(*nvp);
    }
    return re;
}
qint64 TMTCMessage::getTime() const {
    return this->time;
}
bool TMTCMessage::isSpecial() const {
    const int count = this->size();
    
    int cc;
    for (cc = 0; cc < count; cc++){
        TMTCNameValue* nvp = this->at(cc);

        if (!nvp->isSpecial()){

            return true;
        }
    }
    return (0 < count);
}
bool TMTCMessage::hasIdentifier() const {

    return (0 != sid);
}
bool TMTCMessage::hasNotIdentifier() const {

    return (0 == sid);
}
const SystemDeviceIdentifier& TMTCMessage::getIdentifier() const {
    if (0 != sid)
        return *sid;
    else
        return *SystemDeviceIdentifier::BroadcastIdentifier;
}
bool TMTCMessage::setIdentifier(SystemDeviceIdentifier* sid){
    if (0 == this->sid){
        this->sid = sid;
        return true;
    }
    else if (0 == sid){
        this->sid = 0;
        return true;
    }
    else
        return false;
}
