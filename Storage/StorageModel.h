/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageModel_H
#define _CONSOLE_STORAGE_StorageModel_H

#include <QObject>

#include "StorageInterface.h"
#include "PropertyEditorInterface.h"
#include "Null.h"

/*!
 * A storage model is an editable node of the meta object tree that is
 * not a member of a list of peers.  It implements the \class
 * StorageInterface.
 */
class StorageModel : 
    public QObject,
    public StorageInterface, 
    public PropertyEditorInterface
{
 public:
    StorageModel(QObject* parent = vnul)
        : QObject(parent)
    {
    }
};

#endif
