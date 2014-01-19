/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemCatalogInput_H
#define _CONSOLE_SYSTEM_SystemCatalogInput_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "SystemCatalog.h"
#include "SystemCatalogProperties.h"
#include "SystemConnector.h"

/*!
 * Properties and processes for catalog (configuration meta data)
 * input (software system initialization process).
 * 
 * Each implementor of \class SystemCatalogNode must call one of the
 * methods 'sender' or 'receiver' on processing its catalog element
 * from the \class SystemCatalogNode 'read' method.  
 * 
 * This requirement holds for elements having an attribute named "id".
 * The value of the "id" attribute is the string argument of the same
 * name.
 * 
 * An implementor of \class SystemCatalogNode calls the 'sender'
 * method of this class for each child element named "connect".  See
 * \class SystemConnector for the "connect" element attribute names.
 * 
 * Otherwise the system node must call the receiver method (once) when
 * it has an "id" attribute value.
 */
class SystemCatalogInput : public SystemCatalogProperties {

    mutable QList<SystemConnector*> senders;
    mutable QMap<QString,QObject*> receivers;

 public:
    /*!
     */
    SystemCatalogInput();
    /*!
     */
    SystemCatalogInput(SystemCatalog::Property n, QVariant v);
    /*!
     */
    ~SystemCatalogInput();
    /*!
     */
    const QList<SystemConnector*>& getSenders() const;
    /*!
     */
    const QMap<QString,QObject*>& getReceivers() const;
    /*!
     * An implementor of \class SystemCatalogNode calls this method
     * once for each child element named "connect".
     * 
     * \sa SystemConnector
     */
    void sender(const QString& senderId, QObject* sender, 
                const QString& receiverId, 
                const QString& signal, const QString& slot) const;
    /*!
     * An implementor of \class SystemCatalogNode calls this method
     * once when it has no child element named "connect".
     */
    void receiver(const QString id, QObject* receiver) const;
    /*!
     */
    void postprocessing() const;

};
#endif
