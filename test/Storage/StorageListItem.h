/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageListItem_H
#define _CONSOLE_STORAGE_StorageListItem_H

#include <QObject>
#include <QSqlQuery>

#include "ObjectTree/ObjectTreeNode.h"
#include "PropertyEditorInterface.h"

/*!
 * A storage list item is an editable node of the Meta Object Tree
 * that is a member of a list of peers.  Its direct tree ancestor (any
 * member of the set of nodes available by the \QObject parent method)
 * implements the \class StorageInterface.
 * 
 * This interface may be implemented by members of a \class QObject
 * tree that have USER properties for editing, but do not implement
 * \class StorageInterface themselves.
 */
class StorageListItem : 
    public ObjectTreeNode, 
    public PropertyEditorInterface
{

 public:
    /*!
     */
    StorageListItem(QObject* parent = 0)
        : ObjectTreeNode(parent)
    {
    }
    /*!
     * Has no data
     */
    virtual bool isInert() = 0;
    /*!
     * Retrieve bindings by index as for declared user properties.
     */
    virtual void read(QSqlQuery& query, int start = 0) = 0;
    /*!
     * Store bindings by index as for declared user properties.
     */
    virtual void write(QSqlQuery& query, int start = 0) = 0;
};

#endif
