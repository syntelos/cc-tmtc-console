/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemDeviceConnection.h"

SystemDeviceConnection::SystemDeviceConnection(const SystemDeviceIdentifier& sid, QObject* parent)
    : QThread(parent), deviceIdentifier(sid), sendFlag(false), shutdown_flag(false)
{
    this->sendMutex.unlock();
}
SystemDeviceConnection::~SystemDeviceConnection()
{
    this->shutdown();

    this->wait(1000L);
}
void SystemDeviceConnection::shutdown(){
    this->shutdown_flag = true;
}
const SystemDeviceIdentifier& SystemDeviceConnection::getSystemDeviceIdentifier() const {
    return deviceIdentifier;
}
void SystemDeviceConnection::send(const SystemMessage* m){
    if (m){
        const SystemDeviceIdentifier& sid = m->getIdentifier();

        if (sid.isNotValid() || sid == deviceIdentifier){
            /*
             * Permit broadcast style operation with an invalid 'sid'
             */
            this->sendMutex.lock();

            this->sendQ.push_back(new SystemMessage(*m));

            this->sendFlag = true;

            this->sendMutex.unlock();
        }
    }
}
