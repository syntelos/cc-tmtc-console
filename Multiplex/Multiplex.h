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
#include "MultiplexTables.h"
#include "MultiplexSelect.h"

/*!
 * 
 */
class Multiplex : public QObject {
    Q_OBJECT

    MultiplexTables state;

 public:
    Multiplex(QObject* parent = 0);
    ~Multiplex();
    /*!
     * Update the time series data set.  The caller is responsible for
     * the heap allocation of the arguments, which are copied by this
     * method.
     */
    bool update(const SystemDeviceIdentifier*, const TMTCMessage*);
    /*!
     * Query the time series data set.  The caller is responsible for
     * the returned heap allocation.
     */
    TMTCMessage* query(const SystemDeviceIdentifier*, const TMTCMessage*);
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    QVariant query(const SystemDeviceIdentifier&, const TMTCName&);
    /*!
     * Plot selector builds the requested set of path structures with
     * visualization into the requested window.
     */
    void select(int count, MultiplexSelect** query, const QRectF& window);

 signals:
    /*!
     * The database emits this signal to transmit to a device
     * connection.
     */
    void sendToDevice(const SystemDeviceIdentifier*, const TMTCMessage*);
    /*!
     * The database emits this signal to transmit to a user interface
     * (e.g. Terminal Output)
     */
    void sendToUser(const SystemDeviceIdentifier*, const TMTCMessage*);

 public slots:
    /*!
     * The database receives this signal from a device connection to
     * update the time series.
     */
    void receivedFromDevice(const SystemDeviceIdentifier*, const TMTCMessage*);
    /*!
     * The database receives this signal from a user interface
     * (e.g. Terminal Input) to query the time series.  
     * 
     * The identifier is assumed to have persistent allocation, and
     * will be employed in the response signal sent to users.
     */
    void receivedFromUser(const SystemDeviceIdentifier*, const TMTCMessage*);


 private:
    Q_DISABLE_COPY(Multiplex);
};

#endif
