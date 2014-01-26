/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_OBJECTTREE_ObjectTreeImportExport_H
#define _CONSOLE_OBJECTTREE_ObjectTreeImportExport_H

/*!
 * A node in a \class QObject tree implements this interface if it is
 * capable of reading and writing file content.
 */
class ObjectTreeImportExport {

 public:
    /*!
     * Update node from file system
     */
    virtual void importToObjectTreeNode() = 0;
    /*!
     * Save node to file system
     */
    virtual void exportFromObjectTreeNode() = 0;

};

#endif
