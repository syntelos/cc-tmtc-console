/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemDeviceConstructorDiscovery.h"

SystemDeviceConstructorDiscovery::SystemDeviceConstructorDiscovery(const QString& component)
    : plugin(0), library(0)
{
    QLibrary lib(component);
    library = (Construct)lib.resolve("systemDeviceConstructor");
    if (!library){
        QPluginLoader loader(component);
        plugin = dynamic_cast<SystemDeviceConstructor*>(loader.instance());
    }
}
SystemDeviceConstructorDiscovery::~SystemDeviceConstructorDiscovery()
{
    library = 0;
}
bool SystemDeviceConstructorDiscovery::isValid(){
    return (library || plugin);
}
SystemDevice* SystemDeviceConstructorDiscovery::construct(const SystemDeviceIdentifier& s, QObject* p){
    if (library)
        return library(s,p);
    else if (plugin)
        return plugin->construct(s,p);
    else
        return 0;
}
