/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageInterface_H
#define _CONSOLE_STORAGE_StorageInterface_H

#include <QObject>


/*!
 * This interface defined the storage protocol.  It is implemented by
 * nodes of a \class QObject tree that have database connections and
 * perform data fetch and store operations.
 * 
 * The \class PropertyEditorInterface is implemented by nodes of a
 * \class QObject tree that have USER properties for editing.
 * 
 * The \class StorageModel may be subclassed by a node that performs
 * both roles.
 * 
 * The \class StorageListItem may be subclassed by a node that has
 * USER properties but is not performing storage operations.
 * 
 * The editor only requires that a node implement one, both, or none
 * of the interfaces \class StorageInterface or
 * \PropertyEditorInterface.
 */
class StorageInterface {

 signals:
    /*!
     */
    void readSuccess();
    /*!
     */
    void readFailure();
    /*!
     */
    void writeSuccess();
    /*!
     */
    void writeFailure();

 public slots:
    /*!
     * Clear values
     */
    virtual void clear() = 0;
    /*!
     * Read table values
     */
    virtual bool read() = 0;
    /*!
     * Write table values
     */
    virtual bool write() = 0;
    /*!
     * Write for argument zero, otherwise ignore
     */
    virtual bool done(int) = 0;

};

#endif
