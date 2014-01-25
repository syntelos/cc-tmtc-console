/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MEMMULTIPLEX_MemMultiplex_H
#define _CONSOLE_MEMMULTIPLEX_MemMultiplex_H

#include <QObject>

#include "SystemDeviceIdentifier.h"
#include "SystemMultiplexTable.h"
#include "SystemMultiplexTableConstructor.h"

/*!
 * Table class constructor plugin
 */
class MemMultiplex : public QObject, public SystemMultiplexTableConstructor
{
    Q_OBJECT;
    Q_DECLARE_INTERFACE(SystemMultiplexTableConstructor);

 public:
    MemMultiplex();
    ~MemMultiplex();

    virtual SystemMultiplexTable* construct(const SystemDeviceIdentifier&,QObject*);


};
#endif
