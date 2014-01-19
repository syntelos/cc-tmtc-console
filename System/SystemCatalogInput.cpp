/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "SystemCatalogInput.h"

SystemCatalogInput::SystemCatalogInput()
    : SystemCatalogProperties()
{
}
SystemCatalogInput::SystemCatalogInput(SystemCatalog::Property n, QVariant v)
    : SystemCatalogProperties(n,v)
{
}
SystemCatalogInput::~SystemCatalogInput()
{
}
const QMap<QString,SystemConnector*>& SystemCatalogInput::getSenders(){

    return senders;
}
const QMap<QString,QObject*>& SystemCatalogInput::getReceivers(){

    return receivers;
}
void SystemCatalogInput::sender(QString id, QObject* sender, QString signal, QString slot){

    if (sender && !id.isEmpty() && !senders.contains(id)){

        SystemConnector* connector = new SystemConnector(id,sender,signal,slot);

        if (connector->isValidInputSender()){

            senders.insert(id,connector);
        }
    }
}
void SystemCatalogInput::receiver(QString id, QObject* receiver){

    if (!receivers.contains(id)){

        receivers.insert(id,receiver);
    }
}
void SystemCatalogInput::postprocessing(){

    foreach (SystemConnector* connector, senders){

        if (connector->isValidInputSender()){

            QString receiverId(connector->getReceiverId().getSuffix());

            QObject* receiver = receivers.value(receiverId);

            if (receiver){

                if (connector->receiver(receiverId,receiver).connect()){

                    qDebug() << "SystemCatalogProperties.postprocessing: successfully connected" << connector->toString();
                }
                else {

                    qDebug() << "SystemCatalogProperties.postprocessing: failed to connect" << connector->toString();
                }
            }
            else {
                qDebug().nospace() << "SystemCatalogProperties.postprocessing: missing receiver with identifier '" << receiverId << "'";
            }
        }
    }
}
