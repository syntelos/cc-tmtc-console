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
    /*!
     * Message send or receipt time.
     */
    qint64 time;

    SystemDeviceIdentifier* sid;


 public:
    /*!
     * Construct an empty message 
     */
    TMTCMessage();

    TMTCMessage(SystemDeviceIdentifier* sid);
    /*!
     * Construct an empty message with the argument message time
     */
    TMTCMessage(qint64 t);

    TMTCMessage(SystemDeviceIdentifier* sid, qint64 t);
    /*!
     * Parse "NV( NV)*" as {NV,(NV)*}
     */
    TMTCMessage(const QByteArray& in);

    TMTCMessage(SystemDeviceIdentifier* sid, const QByteArray& in);
    /*!
     * Copy a message from a signal when the use of it is not
     * immediate (spans multiple threads).
     */
    TMTCMessage(const TMTCMessage& in);
    /*!
     */
    ~TMTCMessage();
    /*!
     * Field has a value
     */
    bool hasIdentifier() const; 

    bool hasNotIdentifier() const; 
    /*!
     * Return the field value, or the broadcast identifier
     */
    const SystemDeviceIdentifier& getIdentifier() const; 
    /*!
     * Set once (non - null) or unset (null).  Returns false when
     * setting the field that has a non - null value.
     */
    bool setIdentifier(SystemDeviceIdentifier*); 
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
