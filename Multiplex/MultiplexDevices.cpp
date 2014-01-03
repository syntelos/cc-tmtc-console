/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexDevices.h"

MultiplexDevices::MultiplexDevices()
    : QHash()
{
}
MultiplexDevices::MultiplexDevices(const MultiplexDevices& copy)
    : QHash(copy)
{
}
MultiplexDevices::~MultiplexDevices(){
}
