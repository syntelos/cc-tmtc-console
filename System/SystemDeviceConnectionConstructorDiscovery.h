/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemDeviceConnectionConstructorDiscovery_H
#define _CONSOLE_SYSTEM_SystemDeviceConnectionConstructorDiscovery_H

#include <QLibrary>
#include <QPluginLoader>
#include <QString>

#include "SystemDeviceConnectionConstructor.h"

/*!
 * Access to the \class SystemDeviceConnectionConstructor via library or plugin.
 * 
 * 
 */
class SystemDeviceConnectionConstructorDiscovery : public SystemDeviceConnectionConstructor {

    SystemDeviceConnectionConstructor* plugin;
    SystemDeviceConnectionConstructor::Construct library;

 public:
    /*!
     * Component name identifies library or plugin
     */
    SystemDeviceConnectionConstructorDiscovery(const QString& component);
    ~SystemDeviceConnectionConstructorDiscovery();
    /*!
     * Optional test for available library or plugin
     */
    bool isValid();
    /*!
     * Return non null connection if possible
     */
    virtual SystemDeviceConnection* construct(const SystemDeviceIdentifier&,QObject*);
};
#endif
