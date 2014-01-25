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
    virtual qint64 getTime() const = 0;
    /*!
     */
    virtual bool contains(const SystemName&) const = 0;
    /*!
     */
    virtual QVariant getValue(const SystemName&) const = 0;
    /*!
     * Return success (or failure)
     */
    virtual bool setValue(const SystemName&, const QVariant&) = 0;

};
#endif
