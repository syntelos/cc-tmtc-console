/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MULTIPLEX_MultiplexRecord_H
#define _CONSOLE_MULTIPLEX_MultiplexRecord_H

#include <Qt>

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
    /*!
     * Confirm integrity and value
     */
    bool validate(const quint8 value);
    /*!
     * Perform integrity check
     */
    bool check();

    void print();
};
/*!
 *
 */
struct MultiplexFieldF {
 private:
    MultiplexFieldF();
    MultiplexFieldF(MultiplexFieldF&);
    ~MultiplexFieldF();

 public:
    volatile char fs;
    volatile qreal value;
    /*!
     * Constructor
     */
    void init(const qreal value);
    /*!
     * Confirm integrity and value
     */
    bool validate(const qreal value);
    /*!
     * Perform integrity check
     */
    bool check();

    void print();
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
    /*!
     * Confirm integrity and value
     */
    bool validate(const qint64 value);
    /*!
     * Perform integrity check
     */
    bool check();

    void print();
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
    MultiplexFieldF data[];
    /*!
     * Constructor
     */
    void init(const quint8 count);
    /*!
     * Perform limited integrity check
     */
    bool test();
    /*!
     * Perform extended integrity check
     */
    bool check();
    /*!
     * Perform extended integrity check including value of field count
     */
    bool check(const quint8 count);
    /*!
     * Data debug print to stdout.
     */
    void print();

    static int SizeOf(int count);

};

namespace MX {

    enum FieldSizes {
        FieldSizeB = sizeof(MultiplexFieldB), FieldSizeF = sizeof(MultiplexFieldF), FieldSizeL = sizeof(MultiplexFieldL), RecordBase =  sizeof(MultiplexRecord)
    };
};
#endif
