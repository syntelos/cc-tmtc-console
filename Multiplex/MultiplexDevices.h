/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_MultiplexDevices_H
#define _CONSOLE_DATABASE_MultiplexDevices_H

#include <QHash>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "MultiplexDeviceHistory.h"

/*!
 *
 */
class MultiplexDevices : public QHash<SystemDeviceIdentifier,MultiplexDeviceHistory*> {

 public:
    MultiplexDevices();
    MultiplexDevices(const MultiplexDevices& copy);
    ~MultiplexDevices();
};
#endif
