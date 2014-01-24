/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_System_SystemConnectionTCP_H
#define _CONSOLE_System_SystemConnectionTCP_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QTcpSocket>

#include "SystemDeviceConnection.h"
#include "SystemDeviceIdentifier.h"
#include "SystemDevice.h"
#include "SystemMessage.h"

/*!
 * A system device connection for TM/TC over TCP/IP.
 */
class SystemConnectionTCP : public SystemDeviceConnection {
    Q_OBJECT;

 public:
    /*!
     * The argument \a parent is a member of \class QObject and \class
     * SystemDevice
     */
    SystemConnectionTCP(const SystemDeviceIdentifier&,QObject*);
    /*!
     */
    ~SystemConnectionTCP();
    /*!
     */
    virtual void run();
    /*!
     * Cast parent to required \class SystemDevice
     */
    SystemDevice* getSystemDevice() const;

 private:
    Q_DISABLE_COPY(SystemConnectionTCP)
};
#endif
