/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_SystemMultiplexObject_H
#define _CONSOLE_MULTIPLEX_SystemMultiplexObject_H

#include "SystemName.h"

/*!
 * Read/write record data field operator.
 */
class SystemMultiplexObject {

 public:
    /*!
     */
    virtual qint64 getTime() = 0;
    /*!
     */
    virtual bool contains(const SystemName& n) const = 0;
    /*!
     */
    virtual QVariant getValue(const SystemName& n) const = 0;
    /*!
     */
    virtual bool setValue(const SystemName& n, const QVariant& V) = 0;

};
#endif
