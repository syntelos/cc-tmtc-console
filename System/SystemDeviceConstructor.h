/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemDeviceConstructor_H
#define _CONSOLE_SYSTEM_SystemDeviceConstructor_H

#include <QObject>

#include "SystemDevice.h"
#include "SystemDeviceIdentifier.h"

/*!
 * This device plugin interface may be implemented by the typedef'ed
 * library function.
 * 
 * \sa SystemDeviceConstructorDiscovery
 */
struct SystemDeviceConstructor {
    /*!
     * Library function named "systemDeviceConstructor"
     */
    typedef SystemDevice* (*Construct)(const SystemDeviceIdentifier&,QObject*);
    /*!
     * Plugin method
     */
    virtual SystemDevice* construct(const SystemDeviceIdentifier&,QObject*) = 0;

};
#endif
