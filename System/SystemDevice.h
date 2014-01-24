/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDevice_H
#define _SYSTEM_SystemDevice_H

#include "SystemCatalogNode.h"
#include "SystemDeviceConnection.h"
#include "SystemDeviceIdentifier.h"

/*!
 * This interface is implemented by a \class QObject in
 * Configuration/Device, and supplied to to the \class
 * SystemDeviceConnection constructor as a \class QObject parent
 * argument.
 */
class SystemDevice : public SystemCatalogNode {

 public:
    SystemDevice(QObject* p = 0);
    ~SystemDevice();
    /*!
     * Connection identifier parsed for its components.
     */
    virtual const SystemDeviceIdentifier& getSystemDeviceIdentifier() const = 0;
    /*!
     * Retrieve an existing connection as a member of the set of
     * \class QObject children.
     */
    virtual const SystemDeviceConnection* getSystemDeviceConnection() const = 0;

};

#endif
