/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemMultiplexTableConstructor_H
#define _CONSOLE_SYSTEM_SystemMultiplexTableConstructor_H

#include <QObject>

#include "SystemMultiplexTable.h"
#include "SystemDeviceIdentifier.h"

/*!
 * This connection plugin interface may be implemented by the typedef'ed
 * library function.
 * 
 * \sa SystemMultiplexTableConstructorDiscovery
 */
struct SystemMultiplexTableConstructor {
    /*!
     * Library function named "systemMultiplexTableConstructor"
     */
    typedef SystemMultiplexTable* (*Construct)(const SystemDeviceIdentifier&,QObject*);
    /*!
     * Plugin method
     */
    virtual SystemMultiplexTable* construct(const SystemDeviceIdentifier&,QObject*) = 0;

};
#endif
