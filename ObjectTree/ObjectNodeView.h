/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_OBJECTTREE_ObjectNodeView_H
#define _CONSOLE_OBJECTTREE_ObjectNodeView_H

/*!
 * A node editor in a \class QWidget implements this interface if it
 * is capable of updating its data. 
 * 
 * \sa StorageObjectForm
 */
class ObjectNodeView {

 public:
    /*!
     */
    virtual void nodeDataChanged(const char* propname) = 0;

};

#endif
