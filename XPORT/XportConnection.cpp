/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QByteArray>
#include <QDebug>
#include <QString>

#include "System/SystemDeviceIdentifier.h"
#include "XportConnection.h"

XportConnection::XportConnection(QObject* parent)
    : SystemDeviceConnection(parent), sendFlag(false), deviceIdentifier(0)
{
    this->sendMutex.unlock();
    SystemDevice* device = this->getDevice();
    if (device){

        this->deviceIdentifier = const_cast<SystemDeviceIdentifier*>(device->getSystemDeviceIdentifier());
    }
}
XportConnection::~XportConnection(){

    this->shutdown();

    this->wait(1000L);

    if (this->deviceIdentifier){

        delete this->deviceIdentifier;
    }
}
SystemDevice* XportConnection::getDevice(){
    return dynamic_cast<SystemDevice*>(this->parent());
}
SystemDeviceIdentifier* XportConnection::getDeviceIdentifier(){
    return deviceIdentifier;
}
void XportConnection::send(const SystemDeviceIdentifier* sid, const TMTCMessage* m){

    if (sid && m && deviceIdentifier && 
        /*
         * Permit broadcast style operation with an invalid 'sid'
         */
        (sid->isNotValid() || deviceIdentifier->operator==(sid)))
    {

        this->sendMutex.lock();

        this->sendQ.push_back(new TMTCMessage(*m));

        this->sendFlag = true;

        this->sendMutex.unlock();
    }
}
void XportConnection::run(){

    if (deviceIdentifier){

        QTcpSocket socket;

        const QString& prefix = deviceIdentifier->getPrefix();
        const quint16 suffix = deviceIdentifier->getSuffix();

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

                                emit received(deviceIdentifier,msg);
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
    else {
        qDebug() << "XportConnection configuration failed (device identifier)";

        emit configurationFailed();
    }
}
