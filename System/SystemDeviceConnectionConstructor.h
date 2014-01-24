/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemDeviceConnectionConstructor_H
#define _CONSOLE_SYSTEM_SystemDeviceConnectionConstructor_H

#include <QObject>

#include "SystemDeviceConnection.h"
#include "SystemDeviceIdentifier.h"

/*!
 * This connection plugin interface may be implemented by the typedef'ed
 * library function.
 * 
 * \sa SystemDeviceConnectionConstructorDiscovery
 */
struct SystemDeviceConnectionConstructor {
    /*!
     * Library function named "systemDeviceConnectionConstructor"
     */
    typedef SystemDeviceConnection* (*Construct)(const SystemDeviceIdentifier&,QObject*);
    /*!
     * Plugin method
     */
    virtual SystemDeviceConnection* construct(const SystemDeviceIdentifier&,QObject*) = 0;

};
#endif
