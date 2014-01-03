/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageModelRoot_H
#define _CONSOLE_STORAGE_StorageModelRoot_H

#include "ObjectTree/ObjectTreeModel.h"
#include "StorageInterface.h"
#include "PropertyEditorInterface.h"
#include "Null.h"

/*!
 * When the root of the storage tree has storage with USER properties,
 * it would implement this interface.
 */
class StorageModelRoot : 
    public ObjectTreeModel,
    public StorageInterface,
    public PropertyEditorInterface
{
 public:
    /*!
     * The \a parent is typically null.  This class functions
     * identically (as a tree model) with or without a \class QObject
     * parent.
     */
    StorageModelRoot(QObject* parent = vnul) 
        : ObjectTreeModel(parent)
    {
    }

};

#endif
