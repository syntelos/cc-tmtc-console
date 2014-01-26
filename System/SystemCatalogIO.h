/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemCatalogIO_H
#define _CONSOLE_SYSTEM_SystemCatalogIO_H

#include <QObject>
#include <QDomElement>

#include "SystemCatalogInput.h"
#include "SystemCatalogOutput.h"

/*!
 * Procedure for "start" event propagation to children
 * \a SystemCatalogNode
 */
#define _SYSTEM_CATALOG_IO_START(subclass)                             \
    const QObjectList& children = subclass->QObject::children();       \
    const int count = children.count();                                \
    int cc;                                                            \
    for (cc = 0; cc < count; cc++){                                    \
        QObject* child = children.at(cc);                              \
        SystemCatalogIO* node = dynamic_cast<SystemCatalogIO*>(child); \
        if (node){                                                     \
            node->start();                                             \
        }                                                              \
    }
/*!
 * Procedure for "stop" event propagation to children
 * \a SystemCatalogNode
 */
#define _SYSTEM_CATALOG_IO_STOP(subclass)                              \
    const QObjectList& children = subclass->QObject::children();       \
    const int count = children.count();                                \
    int cc;                                                            \
    for (cc = 0; cc < count; cc++){                                    \
        QObject* child = children.at(cc);                              \
        SystemCatalogIO* node = dynamic_cast<SystemCatalogIO*>(child); \
        if (node){                                                     \
            node->stop();                                              \
        }                                                              \
    }
/*!
 * Procedure for "read" call to "properties.sender"
 * \a SystemCatalogNode
 */
#define _SYSTEM_CATALOG_IO_RC(subclass,properties,node,connect) \
    QString senderId = node.attribute("id");                    \
    if (!senderId.isEmpty()){                                   \
                                                                \
        QString senderId2 = connect.attribute("src");           \
                                                                \
        QString receiverId = connect.attribute("dst");          \
        if (!receiverId.isEmpty()){                             \
            QString signal = connect.attribute("signal");       \
            if (!signal.isEmpty()){                             \
                QString slot = connect.attribute("slot");       \
                if (!slot.isEmpty()){                           \
                                                                \
                    properties.sender(senderId,subclass,        \
                                      receiverId,               \
                                      signal,slot);             \
                                                                \
                    return true;                                \
                }                                               \
            }                                                   \
        }                                                       \
    }                                                           \
    return false;

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
class SystemCatalogIO {

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
