/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemConnector_H
#define _CONSOLE_SYSTEM_SystemConnector_H

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QVariant>

#include "SystemConnector.h"
#include "SystemScriptSymbol.h"

/*!
 * Used by \class SystemCatalogInput and \class SystemCatalogOutput to
 * create and reproduce \class QObject "signals and slots" connections
 * (via catalog I/O).
 * 
 * CATALOG OPERATION
 * 
 * The \class SystemCatalogInput method 'postprocessing' creates
 * \class QObject "signals and slots" connections by calling the
 * 'connect' method on this class.  Refer to \class SystemCatalogInput
 * for more information on requirements for all \class
 * SystemCatalogNode implementors.
 * 
 * The \class SystemCatalogOutput provides the method 'listSender' to
 * enable the implementors of \class SystemCatalogNode to produce
 * "connect" elements for their input process.
 * 
 * The "connect" element shall employ the attributes "receiver",
 * "signal" and "slot" for the receiver (catalog element "id"), signal
 * and slot strings defined here.
 * 
 * RUNTIME
 * 
 * The \class SystemConnector stores catalog output information into
 * the sender's \class QObject dynamic property map.  The property
 * name is produced by the method of this class named 'name', and the
 * property value is produced by the method of this class named
 * 'value'.
 * 
 * Instances of this class will produce inutile names and values from
 * incomplete signal - slot connection information.  The normal
 * catalog I/O process prevents this case by checking the validity of
 * connector instances.
 * 
 */
class SystemConnector {

    enum StorageCode {
        GS = 0x1D, RS = 0x1E, FS = 0x1C
    };

    QObject* senderObject;
    QObject* receiverObject;

    SystemScriptSymbol senderId;
    SystemScriptSymbol receiverId;

    SystemScriptSymbol signal;
    SystemScriptSymbol slot;
    /*!
     *
     */
    mutable bool connected;
    /*!
     * Character buffer for 'name' method return value (char*).
     */
    mutable QByteArray nbuf;

 public:
    static const char *NamePrefix;
    /*!
     */
    SystemConnector();
    SystemConnector(const SystemConnector&);
    SystemConnector(const QByteArray&);
    SystemConnector(const QString& signal, const QString& slot);
    SystemConnector(const QString& id, QObject* sender);
    SystemConnector(const QString& id, QObject* sender, const QString& signal);
    SystemConnector(const QString& id, QObject* sender, const QString& signal, const QString& slot);
    SystemConnector(const QString& senderId, QObject* sender, 
                    const QString& signal, const QString& slot,
                    const QString& receiverId, QObject* receiver);
    ~SystemConnector();
    /*!
     */
    SystemConnector& sender(const QString& id, QObject* sender);
    /*!
     */
    SystemConnector& receiver(const QString& id, QObject* receiver);
    /*!
     * Whether the sender is known to be connected with this
     * connector.  This is true when the instance is in the "valid
     * input" state, and the sender has a dynamic property for this
     * connector which is a valid \class QVariant.  The value of the
     * sender's dynamic property is not checked for completeness, as
     * the property existence test (alone) is adequate to the process
     * implemented in this package.
     */
    bool isConnected() const;
    /*!
     * Has complete catalog input information for creating a new
     * signal - slot connection.  All fields required.
     */
    bool isValidInput() const;
    /*!
     * Has partial catalog input information that will be complete
     * with the addition of receiver information.
     * 
     * \sa receiver(const QString&,QObject*)
     */
    bool isValidInputSender() const;
    /*!
     * Has complete catalog output information for reproducing a
     * signal - slot connection to catalog output.
     */
    bool isValidOutput() const;
    /*!
     * Input post processing method to create a \class QObject signal
     * - slot connection.
     * 
     * \sa isValidInput()
     */
    bool connect();
    /*!
     */
    const SystemScriptSymbol& getSenderId() const;
    /*!
     */
    const SystemScriptSymbol& getReceiverId() const;
    /*!
     */
    const SystemScriptSymbol& getSignal() const;
    /*!
     */
    SystemConnector& setSignal(const QString& signal);
    /*!
     */
    const SystemScriptSymbol& getSlot() const;
    /*!
     */
    SystemConnector& setSlot(const QString& slot);
    /*!
     * ASCII string encoded connection name.  This is the sender
     * (\class QObject) dynamic property name
     */
    const char* name() const;
    /*!
     * Binary encoded output data may be incomplete.  This is the
     * sender (\class QObject) dynamic property value.
     */
    const QVariant value() const;
    /*!
     */
    QString toString() const;
};
#endif
