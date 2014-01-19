/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemCatalogNode_H
#define _CONSOLE_SYSTEM_SystemCatalogNode_H

#include <QObject>
#include <QDomElement>

#include "SystemCatalogInput.h"
#include "SystemCatalogOutput.h"

/*!
 * A runtime data - operational class is a member of this class.  Its
 * instances may be called system nodes.  Each system node is
 * configured by an XML element which is emitted and absorbed through
 * DOM I/O processes defined in the system node and its parent node.
 * 
 * A catalog node may be defined as an XML DOM node that configures a
 * system node.  The DOM node is completely defined by the system node
 * which reads and writes it.
 * 
 * Implementors may include the header for \class SystemCatalog for
 * convenience in referencing all of the related classes.
 *
 * \sa SystemCatalog
 * \sa SystemCatalogProperties
 */
class SystemCatalogNode {

 protected:
    /*!
     * Propagate start event to children
     */
    void start(QObject* subclass);
    /*!
     * Propagate stop event to children
     */
    void stop(QObject* subclass);
    /*!
     * Returns whether input sender was called (connect element
     * complete).  
     * 
     * If the input sender method is not called during the processing
     * of all of the child elements of the caller, then the caller
     * must call input receiver.
     * 
     * The process implemented by this method assumes that the second
     * element argument is an element named "connect", and is a child
     * of the first element argument.
     */
    bool readConnect(QObject* subclass, const SystemCatalogInput&, 
                     const QDomElement&, const QDomElement&);

 public slots:
    /*!
     * Occurs after 'read' to start threads.
     */
    virtual void start() = 0;
    /*!
     * Occurs after 'start' to stop threads.
     */
    virtual void stop() = 0;
    /*!
     * Occurs before 'start' to define this system node from the
     * argument node and properties.
     * 
     * \sa SystemCatalogInput
     * \sa SystemConnector
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&) = 0;
    /*!
     * Occurs anytime to export this system node into the argument
     * catalog node according to the publishing properties.
     * 
     * \sa SystemCatalogOutput
     * \sa SystemConnector
     */
    virtual void write(SystemCatalogOutput&, QDomElement&) = 0;

};

#endif
