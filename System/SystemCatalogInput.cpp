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
const QList<SystemConnector*>& SystemCatalogInput::getSenders() const {

    return senders;
}
const QMap<QString,QObject*>& SystemCatalogInput::getReceivers() const {

    return receivers;
}
void SystemCatalogInput::sender(const QString& senderId, QObject* sender, 
                                const QString& receiverId,
                                const QString& signal, const QString& slot) const
{

    if (sender && !senderId.isEmpty()){

        SystemConnector* connector = new SystemConnector(senderId,sender,
                                                         signal,slot, 
                                                         receiverId);

        if (connector->isValidInputSender()){

            senders.append(connector);
        }

        if (!receivers.contains(senderId)){

            receivers.insert(senderId,sender);
        }
    }
}
void SystemCatalogInput::receiver(QString id, QObject* receiver) const {

    if (!receivers.contains(id)){

        receivers.insert(id,receiver);
    }
}
void SystemCatalogInput::postprocessing() const {

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
