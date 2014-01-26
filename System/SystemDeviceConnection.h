/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDeviceConnection_H
#define _SYSTEM_SystemDeviceConnection_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QThread>

#include "SystemCatalogIO.h"
#include "SystemDeviceIdentifier.h"
#include "SystemMessage.h"

/*!
 * A polymorphic \class QObject child of \class SystemDevice.
 * 
 * \section extension Extension
 * 
 * Subclasses implement the run method.
 * 
 * \section catalog Catalog
 *
 * The subclass catalog (XML DOM) node requires two connections as
 * follows:
 * 
 * \code
 *  connect(connection,"received",multiplex,"receivedFromDevice")
 *
 *  connect(multiplex,"sendToDevice",connection,"send")
 * \endcode
 * 
 * While strictly required, these connections are in the catalog for
 * the primary benefit of preventing duplication, and the secondary
 * benefits of completeness and uniformity.
 */
class SystemDeviceConnection : public QThread,
    public SystemCatalogIO
{
    Q_OBJECT;

 protected:
    const SystemDeviceIdentifier& deviceIdentifier;
    QMutex sendMutex;
    QList<SystemMessage*> sendQ;
    volatile bool sendFlag;
    volatile bool shutdown_flag;
    /*!
     * Propagate start event to children
     */
    void start(QObject* subclass);
    /*!
     * Propagate stop event to children
     */
    void stop(QObject* subclass);
    /*!
     * Returns whether input sender was called (connect element
     * complete).  
     * 
     * If the input sender method is not called during the processing
     * of all of the child elements of the caller, then the caller
     * must call input receiver.
     * 
     * The process implemented by this method assumes that the second
     * element argument is an element named "connect", and is a child
     * of the first element argument.
     */
    bool readConnect(QObject* subclass, const SystemCatalogInput&, 
                     const QDomElement&, const QDomElement&);


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
    /*!
     * \sa SystemCatalogIO
     */
    virtual void start();
    /*!
     * \sa SystemCatalogIO
     */
    virtual void stop();
    /*!
     * \sa SystemCatalogIO
     * \sa SystemCatalogInput
     * \sa SystemConnector
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&);
    /*!
     * \sa SystemCatalogIO
     * \sa SystemCatalogOutput
     * \sa SystemConnector
     */
    virtual void write(SystemCatalogOutput&, QDomElement&);

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
