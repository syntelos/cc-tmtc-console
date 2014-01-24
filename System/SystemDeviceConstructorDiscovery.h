/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemDeviceConstructorDiscovery_H
#define _CONSOLE_SYSTEM_SystemDeviceConstructorDiscovery_H

#include <QLibrary>
#include <QPluginLoader>
#include <QString>

#include "SystemDeviceConstructor.h"

/*!
 * Access to the \class SystemDeviceConstructor via library or plugin.
 * 
 * 
 */
class SystemDeviceConstructorDiscovery : public SystemDeviceConstructor {

    SystemDeviceConstructor* plugin;
    SystemDeviceConstructor::Construct library;

 public:
    /*!
     * Component name identifies library or plugin
     */
    SystemDeviceConstructorDiscovery(const QString& component);
    ~SystemDeviceConstructorDiscovery();

    bool isValid();

    virtual SystemDevice* construct(const SystemDeviceIdentifier&,QObject*);
};
#endif
