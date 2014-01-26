/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_H
#define _CONSOLE_DATABASE_H

#include <QObject>
#include <QRectF>

#include "System/SystemCatalogNode.h"
#include "System/SystemDeviceIdentifier.h"
#include "System/SystemMessage.h"
#include "System/SystemName.h"
#include "MultiplexSelect.h"

/*!
 * 
 */
class Multiplex : public SystemCatalogNode {

 public:
    /*!
     */
    Multiplex(QObject* p = 0);
    /*!
     */
    ~Multiplex();
    /*!
     * Update the time series data set.  The caller is responsible for
     * the heap allocation of the arguments, which are copied by this
     * method.
     */
    virtual bool update(const SystemMessage*) = 0;
    /*!
     * Query the time series data set.  The caller is responsible for
     * the returned heap allocation.
     */
    virtual SystemMessage* query(const SystemMessage*) = 0;
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    virtual QVariant query(const SystemDeviceIdentifier&, const SystemName&) = 0;
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
    void sendToDevice(const SystemMessage*);
    /*!
     * The database emits this signal to transmit to a user interface
     * (e.g. Terminal Output)
     */
    void sendToUser(const SystemMessage*);

 public slots:
    /*!
     * The database receives this signal from a device connection to
     * update the time series.
     */
    virtual void receivedFromDevice(const SystemMessage*) = 0;
    /*!
     * The database receives this signal from a user interface
     * (e.g. Terminal Input) to query the time series.  
     * 
     * The identifier is assumed to have persistent allocation, and
     * will be employed in the response signal sent to users.
     */
    virtual void receivedFromUser(const SystemMessage*) = 0;

};

#endif
