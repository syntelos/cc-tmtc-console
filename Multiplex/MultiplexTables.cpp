/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexTables.h"

MultiplexTables::MultiplexTables()
    : QHash()
{
}
MultiplexTables::MultiplexTables(const MultiplexTables& copy)
    : QHash(copy)
{
}
MultiplexTables::~MultiplexTables(){
}
