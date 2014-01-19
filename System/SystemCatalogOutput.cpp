/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QByteArray>

#include "SystemCatalogOutput.h"

SystemCatalogOutput::SystemCatalogOutput()
    : SystemCatalogProperties()
{
}
SystemCatalogOutput::SystemCatalogOutput(SystemCatalog::Property n, QVariant v)
    : SystemCatalogProperties(n,v)
{
}
SystemCatalogOutput::~SystemCatalogOutput()
{
}
QList<SystemConnector>* SystemCatalogOutput::listSender(QObject* sender, bool invalid){

    QList<SystemConnector>* ref = new QList<SystemConnector>();

    if (sender){
        QList<SystemConnector>& tgt = *ref;

        QList<QByteArray> names = sender->dynamicPropertyNames();

        foreach (const QByteArray& name, names){

            if (name.startsWith(SystemConnector::NamePrefix)){

                QVariant property = sender->property(name.constData());

                if (property.isValid() && property.canConvert(QVariant::ByteArray)){

                    SystemConnector connector(property.toByteArray());

                    if (connector.isValidOutput() || invalid){

                        tgt += connector;
                    }
                }
            }
        }
    }
    return ref;
}
