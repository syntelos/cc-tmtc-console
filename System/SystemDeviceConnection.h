/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDeviceConnection_H
#define _SYSTEM_SystemDeviceConnection_H

#include <QObject>
#include <QThread>

/*!
 * A \class QObject child of \class SystemDevice.
 */
class SystemDeviceConnection : public QThread {

 protected:
    bool shutdown_flag;

 public:
    /*!
     * The argument \a parent is a member of \class QObject and \class
     * SystemDevice
     */
    SystemDeviceConnection(QObject* parent);
    /*!
     * Calls shutdown and wait in order to properly cleanup the
     * thread.
     */
    ~SystemDeviceConnection();
    /*!
     * Raise the shutdown flag to initiate the thread termination
     * process
     */
    void shutdown();

 private:
    Q_DISABLE_COPY(SystemDeviceConnection)
};

#endif
