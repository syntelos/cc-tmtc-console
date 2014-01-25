/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemMultiplexTableConstructorDiscovery.h"

SystemMultiplexTableConstructorDiscovery::SystemMultiplexTableConstructorDiscovery(const QString& component)
    : plugin(0), library(0)
{
    QLibrary lib(component);
    library = (Construct)lib.resolve("systemMultiplexTableConstructor");
    if (!library){
        QPluginLoader loader(component);
        plugin = dynamic_cast<SystemMultiplexTableConstructor*>(loader.instance());
    }
}
SystemMultiplexTableConstructorDiscovery::~SystemMultiplexTableConstructorDiscovery()
{
    library = 0;
}
bool SystemMultiplexTableConstructorDiscovery::isValid(){
    return (library || plugin);
}
SystemMultiplexTable* SystemMultiplexTableConstructorDiscovery::construct(const SystemDeviceIdentifier& s, QObject* p){
    if (library)
        return library(s,p);
    else if (plugin)
        return plugin->construct(s,p);
    else
        return 0;
}
