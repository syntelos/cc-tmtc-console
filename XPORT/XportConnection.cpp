/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QByteArray>
#include <QDebug>
#include <QString>

#include "System/SystemDeviceIdentifier.h"
#include "XportConnection.h"

XportConnection::XportConnection(SystemDevice* parent)
    : SystemDeviceConnection(dynamic_cast<QObject*>(parent)), sendFlag(false), deviceIdentifier(parent->getSystemDeviceIdentifier())
{
    this->sendMutex.unlock();
}
XportConnection::~XportConnection(){

    this->shutdown();

    this->wait(1000L);
}
SystemDevice* XportConnection::getSystemDevice() const {
    return dynamic_cast<SystemDevice*>(this->parent());
}
const SystemDeviceIdentifier& XportConnection::getSystemDeviceIdentifier() const {
    return deviceIdentifier;
}
void XportConnection::send(const TMTCMessage* m){
    if (m){
        const SystemDeviceIdentifier& sid = m->getIdentifier();

        if (sid.isNotValid() || sid == deviceIdentifier){
            /*
             * Permit broadcast style operation with an invalid 'sid'
             */
            this->sendMutex.lock();

            this->sendQ.push_back(new TMTCMessage(*m));

            this->sendFlag = true;

            this->sendMutex.unlock();
        }
    }
}
void XportConnection::run(){

    QTcpSocket socket;

    const QString& prefix = deviceIdentifier.getPrefix();
    const quint16 suffix = deviceIdentifier.getSuffix();

    socket.connectToHost(prefix,suffix);

    qDebug().nospace() << "XportConnection connecting to " << prefix.toAscii().data() << ":" << suffix;

    if (socket.waitForConnected()){

        qDebug().nospace() << "XportConnection connection succeeded ";

        emit connectionSucceeded();

        QByteArray linen;

        while (socket.isValid() && (!this->shutdown_flag)){
            /*
             * Output process
             */
            if (this->sendFlag){

                this->sendMutex.lock();

                TMTCMessage* m = this->sendQ.takeFirst();

                QByteArray* output = m->createOutput();

                output->append('\r');
                output->append('\n');

                socket.write(*output);

                socket.flush();

                delete output;

                delete m;

                this->sendFlag = (!this->sendQ.isEmpty());

                this->sendMutex.unlock();
            }
            /*
             * Input process
             *
             * Thread polling rate throttle
             */
            if (socket.waitForReadyRead(200L)){

                while (socket.canReadLine()){
                    linen = socket.readLine();

                    QByteArray linet = linen.trimmed();

                    if ( ! linet.isEmpty()){

                        TMTCMessage* msg = new TMTCMessage(deviceIdentifier,linet);

                        if ( ! msg->isEmpty()){

                            emit received(msg);
                        }

                        msg->deleteLater();
                    }
                }
            }
        }

        socket.close();

        qDebug() << "XportConnection connection terminated";

        emit connectionTerminated();
    }
    else {

        qDebug() << "XportConnection connection failed";

        emit connectionFailed();
    }
}
