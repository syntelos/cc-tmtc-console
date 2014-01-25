/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemMultiplexTable.h"


SystemMultiplexTable::SystemMultiplexTable(const SystemDeviceIdentifier& id, QObject* parent)
    : QObject(parent), identifier(id)
{
    setObjectName(id.toString("table"));
}
SystemMultiplexTable::~SystemMultiplexTable()
{
}
const SystemDeviceIdentifier& SystemMultiplexTable::getSystemDeviceIdentifier() const {

    return identifier;
}
