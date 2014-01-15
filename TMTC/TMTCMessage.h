/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _TMTC_MESSAGE_H
#define _TMTC_MESSAGE_H

#include <QByteArray>
#include <QList>
#include <QObject>

#include "TMTCNameValue.h"

/*!
 * The telemetry and telecommand message is a list of name - value
 * pairs.  
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

 public:
    /*!
     * Construct an empty message 
     */
    TMTCMessage();
    /*!
     * Construct an empty message with the argument message time
     */
    TMTCMessage(qint64 t);
    /*!
     * Parse "NV( NV)*" as {NV,(NV)*}
     */
    TMTCMessage(const QByteArray& in);
    /*!
     * Copy a message from a signal when the use of it is not
     * immediate (spans multiple threads).
     */
    TMTCMessage(const TMTCMessage& in);
    /*!
     */
    ~TMTCMessage();
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
