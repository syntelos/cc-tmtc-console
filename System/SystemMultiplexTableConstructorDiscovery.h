/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemMultiplexTableConstructorDiscovery_H
#define _CONSOLE_SYSTEM_SystemMultiplexTableConstructorDiscovery_H

#include <QLibrary>
#include <QPluginLoader>
#include <QString>

#include "SystemMultiplexTableConstructor.h"

/*!
 * Access to the \class SystemMultiplexTableConstructor via library or plugin.
 * 
 * 
 */
class SystemMultiplexTableConstructorDiscovery : public SystemMultiplexTableConstructor {

    SystemMultiplexTableConstructor* plugin;
    SystemMultiplexTableConstructor::Construct library;

 public:
    /*!
     * Component name identifies library or plugin
     */
    SystemMultiplexTableConstructorDiscovery(const QString& component);
    ~SystemMultiplexTableConstructorDiscovery();
    /*!
     * Optional test for available library or plugin
     */
    bool isValid();
    /*!
     * Return non null connection if possible
     */
    virtual SystemMultiplexTable* construct(const SystemDeviceIdentifier&,QObject*);
};
#endif
