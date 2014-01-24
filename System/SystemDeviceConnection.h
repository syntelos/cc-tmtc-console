/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDeviceConnection_H
#define _SYSTEM_SystemDeviceConnection_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QThread>

#include "SystemDeviceIdentifier.h"
#include "SystemMessage.h"

/*!
 * A polymorphic \class QObject child of \class SystemDevice.
 * 
 * Subclasses implement the run method.
 */
class SystemDeviceConnection : public QThread {
    Q_OBJECT;

 protected:
    const SystemDeviceIdentifier& deviceIdentifier;
    QMutex sendMutex;
    QList<SystemMessage*> sendQ;
    volatile bool sendFlag;
    volatile bool shutdown_flag;


 public:
    /*!
     * The argument \a parent is a member of \class QObject and \class
     * SystemDevice
     */
    SystemDeviceConnection(const SystemDeviceIdentifier&,QObject*);
    /*!
     * Calls shutdown and wait in order to properly cleanup the
     * thread.
     */
    ~SystemDeviceConnection();
    /*!
     * A subclass implements this method to write messages from the
     * send queue, and to emit messages received.
     */
    virtual void run() = 0;
    /*!
     * Raise the shutdown flag to initiate the thread termination
     * process
     */
    void shutdown();
    /*!
     */
    const SystemDeviceIdentifier& getSystemDeviceIdentifier() const;


 public slots:
    /*!
     * Filter receipts for a matching identifier.
     *
     * The caller is responsible for the allocation of the arguments.
     * This class will copy and enqueue the message for network
     * delivery.
     */
    void send( const SystemMessage*);

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
    void received( const SystemMessage*);

 private:
    Q_DISABLE_COPY(SystemDeviceConnection)
};

#endif
