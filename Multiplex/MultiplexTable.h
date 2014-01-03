/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_MultiplexTable_H
#define _CONSOLE_DATABASE_MultiplexTable_H

#include <QHash>
#include <QVariant>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "TMTC/TMTCName.h"
#include "TMTC/TMTCNameValue.h"

/*!
 *
 */
class MultiplexTable : public QHash<TMTCName,QVariant*> {

    const SystemDeviceIdentifier& id;

    qint64 time;

 public:
    static const QVariant Nil;
    /*!
     */
    MultiplexTable(const SystemDeviceIdentifier& id);
    /*!
     */
    MultiplexTable(const MultiplexTable& copy);
    /*!
     */
    ~MultiplexTable();
    /*!
     */
    qint64 getTime();
    /*!
     */
    TMTCMessage* query(const TMTCMessage& m);
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    const QVariant& query(const TMTCName&);
    /*!
     */
    void update(const TMTCMessage& m);
    /*!
     */
    void update(const TMTCNameValue& nvp);

};
#endif
