/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

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
void SystemDeviceConnection::start(QObject* subclass){
    _SYSTEM_CATALOG_IO_START(subclass);
}
void SystemDeviceConnection::stop(QObject* subclass){
    _SYSTEM_CATALOG_IO_STOP(subclass);
}
bool SystemDeviceConnection::readConnect(QObject* subclass,
                                    const SystemCatalogInput& properties, 
                                    const QDomElement& node, 
                                    const QDomElement& connect)
{
    _SYSTEM_CATALOG_IO_RC(subclass,properties,node,connect);
}
void SystemDeviceConnection::start(){
    start(this);
}
void SystemDeviceConnection::stop(){
    stop(this);
}
void SystemDeviceConnection::read(const SystemCatalogInput& properties, const QDomElement& node){
    QString nodeName = node.localName();
    if (nodeName == "connect"){

        QString nodeId = node.attribute("id");

        QDomNodeList children = node.childNodes();
        const uint count = children.length();
        bool inputReceiver = true;

        uint cc;
        for (cc = 0; cc < count; cc++){
            QDomNode child = children.item(cc);
            if (child.isElement()){

                QString name = child.localName();

                QDomElement cel = child.toElement();

                if (name == "connect"){

                    if (readConnect(this,properties,node,cel)){

                        inputReceiver = false;
                    }
                }
                else {
                    qDebug() << "Device.read: skipping unrecognized element" << name ;
                }
            }
        }

        if (inputReceiver){

            if (!nodeId.isEmpty()){
                properties.receiver(nodeId,this);
            }
        }
    }
    else {
        qDebug() << "Device.read: Unrecognized node element" << nodeName;
    }

}
void SystemDeviceConnection::write(SystemCatalogOutput& properties, QDomElement& node){
}
