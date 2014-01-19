/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "SystemConnector.h"

const char *SystemConnector::NamePrefix = "SystemConnector(";

SystemConnector::SystemConnector()
    : senderObject(0), receiverObject(0), connected(false)
{
}
SystemConnector::SystemConnector(const SystemConnector& sc)
    : senderObject(sc.senderObject), receiverObject(sc.receiverObject),
      senderId(sc.senderId), receiverId(sc.receiverId), 
      signal(sc.signal), slot(sc.slot), 
      connected(sc.connected)
{
}
SystemConnector::SystemConnector(const QByteArray& ba)
    : senderObject(0), receiverObject(0), connected(false)
{
    /*
     * Very fast parse for ASCII characters
     * 
     * <GS><RS>
     * <FS><N><senderId>{N}
     * <FS><N><receiverId>{N}
     * <FS><N><signal>{N}
     * <FS><N><slot>{N}
     */
    const char* src = ba.constData();
    const int srclen = ba.length();
    if (10 <= srclen){
        int ofs = 0, flen;
        if (GS == src[ofs++] && RS == src[ofs++]){

            if (FS == src[ofs++]){

                flen = src[ofs++];

                if (0 < flen){

                    senderId.set(&src[ofs],flen);

                    ofs += flen;
                }

                if (FS == src[ofs++]){

                    flen = src[ofs++];

                    if (0 < flen){

                        receiverId.set(&src[ofs],flen);

                        ofs += flen;
                    }

                    if (FS == src[ofs++]){

                        flen = src[ofs++];

                        if (0 < flen){

                            signal.set(&src[ofs],flen);

                            ofs += flen;
                        }

                        if (FS == src[ofs++]){

                            flen = src[ofs++];

                            if (0 < flen){

                                slot.set(&src[ofs],flen);

                                //ofs += flen;
                            }
                        }
                    }
                }
            }
        }
    }
}
SystemConnector::SystemConnector(const QString& signal, const QString& slot)
    : senderObject(0), receiverObject(0), 
      signal(signal), slot(slot),
      connected(false)
{
}
SystemConnector::SystemConnector(const QString& id, QObject* sender)
    : senderObject(sender), receiverObject(0), senderId(id),
      connected(false)
{
}
SystemConnector::SystemConnector(const QString& id, QObject* sender, const QString& signal)
    : senderObject(sender), receiverObject(0), senderId(id), 
      signal(signal), slot(slot),
      connected(false)
{
}
SystemConnector::SystemConnector(const QString& id, QObject* sender, const QString& signal, const QString& slot)
    : senderObject(sender), receiverObject(0), senderId(id), 
      signal(signal), slot(slot),
      connected(false)
{
    if (isValidInput()){
        connected = sender->property(name()).isValid();
    }
}
SystemConnector::SystemConnector(const QString& senderId, QObject* sender, 
                                 const QString& signal, const QString& slot,
                                 const QString& receiverId, QObject* receiver)
    : senderObject(sender), receiverObject(receiver), 
      senderId(senderId), receiverId(receiverId),
      signal(signal), slot(slot),
      connected(false)
{
    if (isValidInput()){
        connected = sender->property(name()).isValid();
    }
}
SystemConnector::~SystemConnector()
{
    senderObject = 0;
    receiverObject = 0;
}
bool SystemConnector::isConnected() const {

    return connected;
}
bool SystemConnector::isValidInput() const {

    return (senderObject && receiverObject && 
            senderId.isNotInert() && receiverId.isNotInert() && 
            signal.isNotInert() && slot.isNotInert());
}
bool SystemConnector::isValidInputSender() const {

    return (senderObject && senderId.isNotInert() && 
            signal.isNotInert() && slot.isNotInert());
}
bool SystemConnector::isValidOutput() const {

    return (senderId.isNotInert() && receiverId.isNotInert() && 
            signal.isNotInert() && slot.isNotInert());
}
SystemConnector& SystemConnector::sender(const QString& id, QObject* sender){

    this->senderId.set(id);

    nbuf.clear();

    this->senderObject = sender;

    if (isValidInput())
        connected = sender->property(name()).isValid();
    else
        connected = false;

    return *this;
}
SystemConnector& SystemConnector::receiver(const QString& id, QObject* receiver){

    this->receiverId.set(id);

    nbuf.clear();

    this->receiverObject = receiver;

    if (isValidInput())
        connected = senderObject->property(name()).isValid();
    else
        connected = false;

    return *this;
}
bool SystemConnector::connect(){

    if (connected){

        return true;
    }
    else if (isValidInput())
    {
        const char* signal = this->signal.signal(senderObject->metaObject());

        const char* slot = this->slot.slot(receiverObject->metaObject());

        if (signal && slot){
            /*
             * Connect
             */
            if (QObject::connect(senderObject,signal,receiverObject,slot)){

                connected = true;
                /*
                 * Catalog
                 * 
                 * The name and value arguments are copied into the
                 * sender QObject dynamic properties container.
                 */
                senderObject->setProperty(name(),value());

                return true;
            }
            else {
                return false;
            }
        }
        else {
            qDebug().nospace() << name() << ".connect failed to dereference signal or slot";
        }
    }
    else {
        qDebug().nospace() << name() << ".connect missing operands";
    }
    return false;
}
const SystemScriptSymbol& SystemConnector::getSenderId() const {

    return senderId;
}
const SystemScriptSymbol& SystemConnector::getReceiverId() const {

    return receiverId;
}
const SystemScriptSymbol& SystemConnector::getSignal() const {

    return signal;
}
SystemConnector& SystemConnector::setSignal(const QString& signal){

    this->signal.set(signal);

    nbuf.clear();

    return *this;
}
const SystemScriptSymbol& SystemConnector::getSlot() const {

    return slot;
}
SystemConnector& SystemConnector::setSlot(const QString& slot){

    this->slot.set(slot);

    nbuf.clear();

    return *this;
}
const char* SystemConnector::name() const {

    if (0 == nbuf.length()){

        nbuf += SystemConnector::NamePrefix;

        nbuf += senderId;

        nbuf += ", ";

        nbuf += signal;

        nbuf += ", ";

        nbuf += receiverId;

        nbuf += ", ";

        nbuf += slot;

        nbuf += ')';
    }
    return nbuf.constData();
}
const QVariant SystemConnector::value() const {
    /*
     * Generate
     */
    QByteArray vbuf;

    vbuf += GS;
    vbuf += RS;

    int flen;
    unsigned char clen;

    flen = senderId.length();
    if (0 < flen && 255 > flen){
        clen = flen;

        vbuf += FS;
        vbuf += clen;
        vbuf += senderId;
    }
    else {
        vbuf += FS;
        vbuf += (char)0;
    }

    flen = receiverId.length();
    if (0 < flen && 255 > flen){
        clen = flen;

        vbuf += FS;
        vbuf += clen;
        vbuf += receiverId;
    }
    else {
        vbuf += FS;
        vbuf += (char)0;
    }

    flen = signal.length();
    if (0 < flen && 255 > flen){
        clen = flen;

        vbuf += FS;
        vbuf += clen;
        vbuf += signal;
    }
    else {
        vbuf += FS;
        vbuf += (char)0;
    }

    flen = slot.length();
    if (0 < flen && 255 > flen){
        clen = flen;

        vbuf += FS;
        vbuf += clen;
        vbuf += slot;
    }
    else {
        vbuf += FS;
        vbuf += (char)0;
    }
    /*
     */
    return vbuf;
}
QString SystemConnector::toString() const {

    return name();
}
