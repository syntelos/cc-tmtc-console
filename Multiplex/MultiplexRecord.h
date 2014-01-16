/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexRecord_H
#define _CONSOLE_MULTIPLEX_MultiplexRecord_H

#include <Qt>
#include <QVariant>

namespace MX {

    enum Delimeters {
        GS = 0x1D, RS = 0x1E, FS = 0x1C
    };
};
/*!
 *
 */
struct MultiplexFieldB {
 private:
    MultiplexFieldB();
    MultiplexFieldB(MultiplexFieldB&);
    ~MultiplexFieldB();

 public:
    volatile char fs;
    volatile quint8 value;
    /*!
     * Constructor
     */
    void init(const quint8 value);

    void init(const MultiplexFieldB& copy);
    /*!
     * Confirm integrity and value
     */
    bool validate(const quint8 value) const;
    /*!
     * Perform integrity check
     */
    bool check() const;

    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};
/*!
 * 
 */
struct MultiplexFieldL {
 private:
    MultiplexFieldL();
    MultiplexFieldL(MultiplexFieldL&);
    ~MultiplexFieldL();

 public:
    volatile char fs;
    volatile qint64 value;
    /*!
     * Constructor
     */
    void init(const qint64 value);

    void init(const MultiplexFieldL& copy);
    /*!
     * Confirm integrity and value
     */
    bool validate(const qint64 value) const;
    /*!
     * Perform integrity check
     */
    bool check() const;

    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};
/*!
 * 
 */
struct MultiplexFieldP {
 private:
    MultiplexFieldP();
    MultiplexFieldP(MultiplexFieldP&);
    ~MultiplexFieldP();

 public:
    volatile char fs;
    volatile qptrdiff value;
    /*!
     * Constructor
     */
    void init(const qptrdiff value);

    void init(const MultiplexFieldP& copy);
    /*!
     * Confirm integrity and value
     */
    bool validate(const qptrdiff value) const;
    /*!
     * Perform integrity check
     */
    bool check() const;

    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};
/*!
 * 
 */
struct MultiplexFieldI {
 private:
    MultiplexFieldI();
    MultiplexFieldI(MultiplexFieldI&);
    ~MultiplexFieldI();

 public:
    volatile char fs;
    volatile quint32 value;
    /*!
     * Constructor
     */
    void init(const quint32 value);

    void init(const MultiplexFieldI& copy);
    /*!
     * Confirm integrity and value
     */
    bool validate(const quint32 value) const;
    /*!
     * Perform integrity check
     */
    bool check() const;

    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};

/*!
 *
 */
struct MultiplexFieldV {
 private:
    MultiplexFieldV();
    MultiplexFieldV(MultiplexFieldV&);
    ~MultiplexFieldV();

 public:
    enum ValueAlloc {
        DefaultAlloc = sizeof(double),
        StorageLimit = 255
    };

    volatile char fs;
    volatile quint8 alloc;
    volatile quint8 storage;
    volatile quint8 value[];
    /*!
     * This call has no effect when the argument byte array is too
     * long.  Return false for an argument that is ignored.
     *
     * This proceedure depends on (uses) the value of 'alloc' defined
     * by 'init(const QVariant&)'
     */
    bool setValue(const QVariant& value);
    /*!
     */
    QVariant getValue() const;
    /*!
     * Constructor
     *
     * This proceedure defines the value of 'alloc' used by
     * 'setValue(const QVariant&)'
     */
    bool init(const QVariant& value);
    /*!
     * This proceedure defines the value of 'alloc' required by
     * 'setValue'
     */
    void init(const MultiplexFieldV& copy);
    /*!
     * Confirm integrity and value
     */
    bool validate(const QVariant& value) const;
    /*!
     * Perform integrity check
     */
    bool check() const;

    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};
/*!
 *
 */
struct MultiplexRecord {

 public:
    volatile char gs;
    volatile char rs;
    MultiplexFieldL time;
    MultiplexFieldB count;
    MultiplexFieldV data[];
    /*!
     * Convenience for time value
     */
    qint64 getTime() const;
    /*!
     * Convenience for count value
     */
    quint8 getFieldCount() const;
    /*!
     * Sum of space allocated to fields
     */
    qptrdiff getFieldLength() const;
    /*!
     * Cleaning constructor
     */
    void init();
    /*!
     * Copy constructor
     */
    void init(const MultiplexRecord& copy);
    /*!
     * Perform integrity check of base record: GS, RS, time and count.
     */
    bool check() const;
    /*!
     * Object separators (hard points) have value zero
     */
    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};
/*!
 *
 */
struct MultiplexIndexRecord {

 public:
    volatile char gs;
    volatile char rs;

    MultiplexFieldP object_size;
    MultiplexFieldP ofs_first;
    MultiplexFieldP ofs_last;
    MultiplexFieldI count_temporal;
    MultiplexFieldI count_spatial;
    MultiplexFieldI count_user;
    MultiplexFieldB alloc;
    MultiplexFieldB count;
    MultiplexFieldV data[];

    /*!
     * Convenience for count value
     */
    quint8 getFieldCount() const;
    /*!
     * Sum of space allocated to fields
     */
    qptrdiff getFieldLength() const;
    /*!
     * Cleaning constructor
     */
    void init();
    /*!
     * Perform integrity check of base record: GS, RS, time and count.
     */
    bool check() const;
    /*!
     * Object separators (hard points) have value zero
     */
    bool zero() const;
    /*!
     * With integrity, return the non zero (byte) length of this
     * record (for address arithmetic).
     */
    qptrdiff length() const;

};

namespace MX {
    /*!
     * Addressing constants
     */
    enum Lengths {
        FieldSizeB       = sizeof(MultiplexFieldB),
        FieldSizeV       = sizeof(MultiplexFieldV),
        FieldSizeL       = sizeof(MultiplexFieldL),
        FieldSizeP       = sizeof(MultiplexFieldP),
        FieldSizeI       = sizeof(MultiplexFieldI),
        RecordBase       = sizeof(MultiplexRecord),
        RecordIndexCount = 20,
        RecordIndexBase  = sizeof(MultiplexIndexRecord),
        RecordIndexInit  = (RecordIndexBase + (RecordIndexCount*FieldSizeV) + (RecordIndexCount*255)),
        RecordIndexMax   = (sizeof(MultiplexIndexRecord) + (255*FieldSizeV) + (255*255)),
        RecordMax        = (RecordBase + (255*FieldSizeV) + (255*255))
    };
};
#endif
