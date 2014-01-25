/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _System_MESSAGE_H
#define _System_MESSAGE_H

#include <QByteArray>
#include <QList>
#include <QObject>

#include "SystemDeviceIdentifier.h"
#include "SystemMultiplexObject.h"
#include "SystemNameValue.h"

/*!
 * The telemetry and telecommand message is a list of name - value
 * pairs.  The message has a receipt or delivery time, and a device
 * identifier.  
 * 
 * The identifier refers to the message source or target in receiving
 * or delivering from or to a device.  The message class is not
 * responsible for the heap allocation of the identifier, which is
 * presumed to be relatively static.
 * 
 * The \class QObject has been added for the use of the "deleteLater"
 * facility.  Emitting signal arguments in this class with sender
 * responsibility for heap allocation is implemented by calling
 * "deleteLater" after emitting the signal.
 */
class SystemMessage : public QObject, 
    public QList<SystemNameValue*>,
    public SystemMultiplexObject
{
    Q_OBJECT;

 public:
    /*!
     * Message send or receipt time.
     */
    const qint64 time;
    /*!
     * Message send target, or receipt source
     */
    const SystemDeviceIdentifier& identifier;

    /*!
     * Construct an empty message 
     */
    SystemMessage(const SystemDeviceIdentifier& sid);
    /*!
     * Construct an empty message with the argument message time
     */
    SystemMessage(const SystemDeviceIdentifier& sid, qint64 t);
    /*!
     * Parse "NV( NV)*" as {NV,(NV)*}
     */
    SystemMessage(const SystemDeviceIdentifier& sid, const QByteArray& in);
    /*!
     * Copy a message from a signal when the use of it is not
     * immediate (spans multiple threads).
     */
    SystemMessage(const SystemMessage& in);

    SystemMessage(const SystemMessage& in, qint64 t);
    /*!
     */
    ~SystemMessage();
    /*!
     * Return the field value, or the broadcast identifier
     */
    const SystemDeviceIdentifier& getIdentifier() const; 
    /*!
     * The names contained are all special
     */
    bool isSpecial() const ;
    /*!
     * Message object construction time is copied by the copy
     * constructor.
     */
    virtual qint64 getTime() const ;
    /*!
     * Return a new byte array with the formatted contents of this
     * message.  The caller is responsible for deleting the returned
     * (heap) object.
     */
    QByteArray* createOutput() const ;
    /*!
     */
    int indexOf(const SystemName&) const;
    /*!
     */
    virtual bool contains(const SystemName& n) const;
    /*!
     */
    virtual QVariant getValue(const SystemName& n) const;
    /*!
     */
    virtual bool setValue(const SystemName& n, const QVariant& V);

};

#endif
