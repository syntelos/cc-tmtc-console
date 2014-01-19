/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_H
#define _CONSOLE_DATABASE_H

#include <QObject>
#include <QRectF>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "TMTC/TMTCName.h"
#include "MultiplexSelect.h"

/*!
 * 
 */
class Multiplex {

 public:
    /*!
     * Update the time series data set.  The caller is responsible for
     * the heap allocation of the arguments, which are copied by this
     * method.
     */
    virtual bool update(const TMTCMessage*) = 0;
    /*!
     * Query the time series data set.  The caller is responsible for
     * the returned heap allocation.
     */
    virtual TMTCMessage* query(const TMTCMessage*) = 0;
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    virtual QVariant query(const SystemDeviceIdentifier&, const TMTCName&) = 0;
    /*!
     * Plot selector builds the requested set of path structures with
     * visualization into the requested window.
     */
    virtual void select(int count, MultiplexSelect** query, const QRectF& window) = 0;

 signals:
    /*!
     * The database emits this signal to transmit to a device
     * connection.
     */
    void sendToDevice(const TMTCMessage*);
    /*!
     * The database emits this signal to transmit to a user interface
     * (e.g. Terminal Output)
     */
    void sendToUser(const TMTCMessage*);

 public slots:
    /*!
     * The database receives this signal from a device connection to
     * update the time series.
     */
    virtual void receivedFromDevice(const TMTCMessage*) = 0;
    /*!
     * The database receives this signal from a user interface
     * (e.g. Terminal Input) to query the time series.  
     * 
     * The identifier is assumed to have persistent allocation, and
     * will be employed in the response signal sent to users.
     */
    virtual void receivedFromUser(const TMTCMessage*) = 0;

};

#endif
