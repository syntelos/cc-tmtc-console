/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QObjectList>

#include "Configuration/CCDB.h"
#include "Configuration/HCDB.h"
#include "Configuration/Devices.h"
#include "Configuration/Device.h"
#include "System/SystemDeviceConnection.h"
#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "Init.h"

Init::Init(Configuration* configuration)
    : QList(), configuration(configuration), status(Down), multiplex(0)
{
}
Init::~Init(){
    this->status = Down;
    this->configuration = 0;
    this->multiplex = 0;
    this->clear();
}
bool Init::isUp() const {

    return (Up == this->status);
}
bool Init::isDown() const {

    return (Down == this->status);
}
Init::Status Init::getStatus() const {

    return this->status;
}
Multiplex* Init::getMultiplex() const {

    return this->multiplex;
}
bool Init::run(){
    this->status = Down;
    this->clear();

    if (this->multiplex){

        this->multiplex->setParent(0);
        this->multiplex->deleteLater();
        this->multiplex = 0;
    }

    if (this->configuration){
        /*
         * (element 0)
         */
        this->append(configuration);

        CCDB* ccdb = this->configuration->getCCDB();
        if (ccdb->isReady()){
            /*
             * (element 1)
             */
            this->append(ccdb);

            HCDB* hcdb = this->configuration->getHCDB();
            if (hcdb->isUp()){
                /*
                 * (element 2)
                 */
                this->append(hcdb);

                Devices* devices = hcdb->getDevices();
                if (devices){
                    const QObjectList& children = devices->children();
                    const int count = children.size();
                    if (0 < count){
                        this->multiplex = new Multiplex(this->configuration);
                        this->append(multiplex);
                        int cc;
                        for (cc = 0; cc < count; cc++){

                            Device* device = static_cast<Device*>(children.at(cc));

                            SystemDeviceConnection* connection = const_cast<SystemDeviceConnection*>(device->createSystemDeviceConnection());

                            if (connection &&
                                QObject::connect(connection,SIGNAL(received(const SystemDeviceIdentifier*,const TMTCMessage*)),multiplex,SLOT(receivedFromDevice(const SystemDeviceIdentifier*,const TMTCMessage*))) &&
                                QObject::connect(multiplex,SIGNAL(sendToDevice(const SystemDeviceIdentifier*,const TMTCMessage*)),connection,SLOT(send(const SystemDeviceIdentifier*,const TMTCMessage*)))
                                )
                            {
                                /*
                                 * (element 3)
                                 */
                                this->append(device);
                                /*
                                 */
                                connection->start();
                            }
                            else {
                                /*
                                 * Cleanup immediate
                                 */
                                if (connection){

                                    connection->setParent(0);

                                    delete connection;
                                }
                                /*
                                 * Cleanup previous
                                 */
                                if (0 < cc){
                                    int c0;
                                    for (c0 = 0; c0 < cc; c0++){

                                        device = static_cast<Device*>(children.at(cc));

                                        device->shutdownSystemDeviceConnection();
                                        /*
                                         * (element 3)
                                         */
                                        this->removeAt(3);
                                    }
                                }
                                return false;
                            }
                        }
                        this->status = Up;
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}
