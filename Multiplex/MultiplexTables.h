/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_MultiplexTables_H
#define _CONSOLE_DATABASE_MultiplexTables_H

#include <QHash>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "MultiplexTable.h"

/*!
 *
 */
class MultiplexTables : public QHash<SystemDeviceIdentifier,MultiplexTable*> {

 public:
    MultiplexTables();
    MultiplexTables(const MultiplexTables& copy);
    ~MultiplexTables();
};
#endif
