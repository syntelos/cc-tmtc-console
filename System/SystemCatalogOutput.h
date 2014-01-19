/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemCatalogOutput_H
#define _CONSOLE_SYSTEM_SystemCatalogOutput_H

#include <QList>
#include <QObject>
#include <QVariant>

#include "SystemCatalog.h"
#include "SystemConnector.h"
#include "SystemCatalogProperties.h"

/*!
 * Properties and processes for catalog (configuration meta data)
 * output (software system publication process).
 * 
 * Each implementor of \class SystemCatalogNode employs the "list
 * sender" method to produce "connect" (child) elements.
 */
class SystemCatalogOutput : public SystemCatalogProperties {

 public:
    /*!
     */
    SystemCatalogOutput();
    /*!
     */
    SystemCatalogOutput(SystemCatalog::Property n, QVariant v);
    /*!
     */
    ~SystemCatalogOutput();
    /*!
     * Create a list of (output valid) connectors for the argument
     * sender for output into the system catalog.  The caller is
     * responsible for deleting the returned memory.
     * 
     * The listed (available) connections are those created by \class
     * SystemConnector as via \class SystemCatalogInput.
     * 
     * The optional boolean argument can be employed with value true
     * in order to include invalid connectors in the returned listing.
     * Note that invalid connectors should not be found as the
     * processes implemented in this package will not preserve them.
     */
    QList<SystemConnector>* listSender(QObject*, bool invalid = false);

};
#endif
