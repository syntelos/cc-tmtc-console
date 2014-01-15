/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_MultiplexTable_H
#define _CONSOLE_DATABASE_MultiplexTable_H

#include <Qt>
#include <QFile>
#include <QHash>
#include <QReadWriteLock>
#include <QVariant>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCMessage.h"
#include "TMTC/TMTCName.h"
#include "TMTC/TMTCNameValue.h"
#include "MultiplexRecord.h"
#include "MultiplexSelect.h"

/*!
 * Local TS data cache file in the application working directory.
 * 
 * 
 * OPERATION
 * 
 * The table file operates as a circular list which wraps around at
 * the end to overwrite the beginning.  Each record timestamp occurs
 * in ascending order until the end of the time series wraps into the
 * beginning of the file.
 * 
 * The table record (\class MultiplexRecord) has variable size, but is
 * employed to grow in size as each new record includes the data set
 * of the previous "new record" (most recent in time).  Record address
 * arithmetic employs record object length.
 *
 * Record address offsets will skew arbitrarily within the (N * Object
 * Size) boundaries of the memory mapped data cache (file) due to the
 * dynamic record size.  When a field is first entered into the data
 * cache, its allocated length is defined -- which can occur at any
 * moment in the life of the data cache.  Record address arithmetic is
 * defined in the \class MultiplexTableIterator.
 * 
 * The table file size is defined as record size multiplied by record
 * number.  When the record size changes, the table file size changes
 * and is resized via the reopen method.
 * 
 * 
 * OVERHEAD
 * 
 * The internal, actual storage capacity record count is larger than
 * the user domain record count in order to accomodate two kinds of
 * record storage conflict: temporal and spatial.
 * 
 * The conflict or overhead region is at the start of the time series,
 * where a new update overwrites the oldest record.
 * 
 * In the temporal conflict, concurrent reads and writes in the time
 * series data cache could interfere with the use of the data cache.
 * The number of records necessary to avoid this potential depends on
 * the update frequency and the magnitude of the selection use
 * duration time.  For example, an update frequency of once per second
 * and a selection duration of a few milliseconds would require an
 * overhead of one record to accomodate the probable temporal conflict
 * event (when an update overwrites a selected record).
 * 
 * A spatial class of overhead derives from record object size
 * variation.  The most recent record offset period and size may
 * conflict with the least recent record offset period and size.  A
 * new record may overwrite part (or more) of the least recent record.
 * 
 * The \class MultiplexTable can accomodate spatial conflict and plan
 * accordingly, while the user is expected to define the temporal
 * overhead.
 *
 */
class MultiplexTable {

    const SystemDeviceIdentifier& id;

    MultiplexIndex index;

    QFile file;

    quintptr data;

    QReadWriteLock lock;

    /*!
     * DISABLE COPY -- destructor unmaps data
     */
    MultiplexTable(const MultiplexTable& copy);
    /*!
     * Expand file map as required by record count and object size.
     * Implicitly (unsafely) depends on positive object-size and open
     * table
     */
    void reopen();
    /*!
     */
    MultiplexRecord* record(quintptr p);
    /*!
     */
    MultiplexRecord* recordNew();
    /*!
     */
    MultiplexRecord* recordFirst();
    /*!
     */
    MultiplexRecord* recordLast();

 public:
    /*!
     */
    MultiplexTable(const SystemDeviceIdentifier& id);
    /*!
     */
    ~MultiplexTable();
    /*!
     * Table is open
     */
    bool isOpen();
    /*!
     */
    void close();
    /*!
     */
    bool open();
    /*!
     */
    quint32 getOverheadTemporal();
    /*!
     */
    void setOverheadTemporal(quint32);
    /*!
     */
    quint32 getOverheadSpatial();
    /*!
     */
    void setOverheadSpatial(quint32);
    /*!
     */
    quint32 getRecordCount();
    /*!
     * The external number of records in the application domain,
     * independent of overhead requirements.
     */
    void setRecordCount(quint32);
    /*!
     */
    TMTCMessage* query(const TMTCMessage& m);
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    QVariant query(const TMTCName&);
    /*!
     * Create a new record.  If the cache is not open, the operation
     * of the multiplex will populate the index.
     */
    void update(const TMTCMessage& m);
    /*!
     */
    void update(const TMTCNameValue& nvp);

    void select(MultiplexSelect& select);

};
#endif
