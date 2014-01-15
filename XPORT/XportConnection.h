/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_XPORT_XportConnection_H
#define _CONSOLE_XPORT_XportConnection_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QTcpSocket>

#include "System/SystemDeviceConnection.h"
#include "System/SystemDeviceIdentifier.h"
#include "System/SystemDevice.h"
#include "TMTC/TMTCMessage.h"

/*!
 * 
 */
class XportConnection : public SystemDeviceConnection {
    Q_OBJECT;

    QMutex sendMutex;
    QList<TMTCMessage*> sendQ;
    volatile bool sendFlag;
    SystemDeviceIdentifier* deviceIdentifier;

 public:
    /*!
     * The argument \a parent is a member of \class QObject and \class
     * SystemDevice
     */
    XportConnection(QObject* parent);
    ~XportConnection();

    void run();
    /*!
     * Cast parent to required \class SystemDevice
     */
    SystemDevice* getDevice();

    SystemDeviceIdentifier* getDeviceIdentifier();

 public slots:
    /*!
     * Filter receipts for a matching identifier.
     *
     * The caller is responsible for the allocation of the arguments.
     * This class will copy and enqueue the message for network
     * delivery.
     */
    void send( const TMTCMessage*);

 signals:
    /*!
     * The parent device and connection identifier are present and valid
     */
    void configurationSucceeded();
    /*!
     * The parent device or connection identifier is missing or invalid
     */
    void configurationFailed();
    /*!
     * The initial network connection has succeeded
     */
    void connectionSucceeded();
    /*!
     * The initial network connection has failed
     */
    void connectionFailed();
    /*!
     * An established network connection has been closed.
     */
    void connectionTerminated();
    /*!
     * Message received signal.
     * 
     * This class is responsible for managing the allocation of the
     * arguments.  The message argument is deleted immediately
     * following the emission of the signal.
     */
    void received( const TMTCMessage*);

 private:
    Q_DISABLE_COPY(XportConnection)
};
#endif
