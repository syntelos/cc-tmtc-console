/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDateTime>

#include "TMTCMessage.h"

/*!
 */
TMTCMessage::TMTCMessage(const SystemDeviceIdentifier& sid)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), identifier(sid)
{
}
/*!
 */
TMTCMessage::TMTCMessage(const SystemDeviceIdentifier& sid, qint64 time)
    : QObject(), QList(), time(time), identifier(sid)
{
}
/*!
 * Parse "NV( NV)*" as {NV,(NV)*}
 */
TMTCMessage::TMTCMessage(const SystemDeviceIdentifier& sid, const QByteArray& in)
    : QObject(), QList(), time(QDateTime::currentMSecsSinceEpoch()), identifier(sid)
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
    : QObject(), QList(), time(in.time), identifier(in.identifier)
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
const SystemDeviceIdentifier& TMTCMessage::getIdentifier() const {

    return identifier;
}
