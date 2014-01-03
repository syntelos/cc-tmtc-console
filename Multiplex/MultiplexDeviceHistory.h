/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_MultiplexDeviceHistory_H
#define _CONSOLE_DATABASE_MultiplexDeviceHistory_H

#include <QList>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "TMTC/TMTCName.h"
#include "TMTC/TMTCNameValue.h"
#include "MultiplexTable.h"
#include "MultiplexSelect.h"

/*!
 *
 */
class MultiplexDeviceHistory : public QList<MultiplexTable*> {

    const SystemDeviceIdentifier& id;

 public:
    /*!
     */
    MultiplexDeviceHistory(const SystemDeviceIdentifier& id);
    /*!
     */
    ~MultiplexDeviceHistory();
    /*!
     */
    TMTCMessage* query(const TMTCMessage* m);
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    const QVariant& query(const TMTCName&);
    /*!
     */
    bool update(const TMTCMessage* m);
    /*!
     */
    void update(const TMTCNameValue& nvp);
    /*!
     */
    void select(MultiplexSelect& s);

};
#endif
