/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemDeviceConnectionConstructorDiscovery.h"

SystemDeviceConnectionConstructorDiscovery::SystemDeviceConnectionConstructorDiscovery(const QString& component)
    : plugin(0), library(0)
{
    QLibrary lib(component);
    library = (Construct)lib.resolve("systemDeviceConnectionConstructor");
    if (!library){
        QPluginLoader loader(component);
        plugin = dynamic_cast<SystemDeviceConnectionConstructor*>(loader.instance());
    }
}
SystemDeviceConnectionConstructorDiscovery::~SystemDeviceConnectionConstructorDiscovery()
{
    library = 0;
}
bool SystemDeviceConnectionConstructorDiscovery::isValid(){
    return (library || plugin);
}
SystemDeviceConnection* SystemDeviceConnectionConstructorDiscovery::construct(const SystemDeviceIdentifier& s, QObject* p){
    if (library)
        return library(s,p);
    else if (plugin)
        return plugin->construct(s,p);
    else
        return 0;
}
