/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _TMTC_MESSAGE_H
#define _TMTC_MESSAGE_H

#include <QByteArray>
#include <QList>
#include <QObject>

#include "System/SystemDeviceIdentifier.h"
#include "TMTCNameValue.h"

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
class TMTCMessage : public QObject, public QList<TMTCNameValue*> {
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
    TMTCMessage(const SystemDeviceIdentifier& sid);
    /*!
     * Construct an empty message with the argument message time
     */
    TMTCMessage(const SystemDeviceIdentifier& sid, qint64 t);
    /*!
     * Parse "NV( NV)*" as {NV,(NV)*}
     */
    TMTCMessage(const SystemDeviceIdentifier& sid, const QByteArray& in);
    /*!
     * Copy a message from a signal when the use of it is not
     * immediate (spans multiple threads).
     */
    TMTCMessage(const TMTCMessage& in);
    /*!
     */
    ~TMTCMessage();
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
    qint64 getTime() const ;
    /*!
     * Return a new byte array with the formatted contents of this
     * message.  The caller is responsible for deleting the returned
     * (heap) object.
     */
    QByteArray* createOutput() const ;
};

#endif
