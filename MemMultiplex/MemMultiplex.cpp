/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MemMultiplex.h"

MemMultiplex::MemMultiplex()
{
}
MemMultiplex::~MemMultiplex()
{
}
SystemMultiplexTable* MemMultiplex::construct(const SystemDeviceIdentifier& s,QObject* p){

    return new MemMultiplex(s,p);
}
