/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexObject_H
#define _CONSOLE_MULTIPLEX_MultiplexObject_H

#include "TMTC/TMTCName.h"
#include "MultiplexIndex.h"
#include "MultiplexRecord.h"
/*!
 * High level (read only) record operator.
 *
 * \sa MultiplexRecordIterator
 */
class MultiplexObject {

 protected:
    const MultiplexIndex& index;
    const MultiplexRecord& record;

 public:
    /*!
     */
    MultiplexObject(const MultiplexIndex& index, const MultiplexRecord& record);
    /*!
     */
    ~MultiplexObject();
    /*!
     */
    bool contains(const TMTCName& n) const;
    /*!
     */
    qint64 getTime() const;
    /*!
     */
    QVariant operator[](const TMTCName& n) const;

};
#endif
