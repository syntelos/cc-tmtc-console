/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDevice_H
#define _SYSTEM_SystemDevice_H

#include "SystemDeviceIdentifier.h"
#include "SystemDeviceConnection.h"

/*!
 * This interface is implemented by a \class QObject in
 * Configuration/Device, and supplied to to the \class
 * SystemDeviceConnection constructor as a \class QObject parent
 * argument.
 */
class SystemDevice {

 public:
    /*!
     * Connection address/port identifier string.  A TCP/IP connection
     * may parse this string for a port number suffix separated by a
     * colon (':') character (from a DNS host name or IP address
     * prefix).
     */
    virtual const QString* getConnectionIdentifier() const = 0;
    /*!
     * Connection identifier parsed for its components.
     * 
     * The caller is responsible for the returned heap allocation.
     */
    virtual const SystemDeviceIdentifier* getSystemDeviceIdentifier() const = 0;
    /*!
     * Retrieve an existing connection as a member of the set of
     * \class QObject children.
     */
    virtual const SystemDeviceConnection* getSystemDeviceConnection() const = 0;

};

#endif
