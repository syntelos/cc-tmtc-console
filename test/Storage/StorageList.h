/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageList_H
#define _CONSOLE_STORAGE_StorageList_H

#include <QObject>

#include "ObjectTree/ObjectTreeList.h"
#include "ObjectTree/ObjectTreeNode.h"
#include "StorageInterface.h"

/*!
 * Abstract interface to subjects of the \class
 * StorageEditor.
 * 
 * The \class QObject children of members of this type have one type,
 * implement \class StorageListItem, and define meta
 * object properties for USER properties.

 * Members of this type have no USER properties.
 */
template <class T>
class StorageList : public ObjectTreeNode,
    public StorageInterface,
    public ObjectTreeList
{

 public:
    /*!
     */
    StorageList(QObject* parent = 0)
        : ObjectTreeNode(parent)
    {
    }

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
     * Read table values, destroying any previously contained
     * children.
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
