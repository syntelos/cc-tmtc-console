/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include <QMetaType>

#include "SystemCatalogNode.h"

SystemCatalogNode::SystemCatalogNode(QObject* p)
    : QObject(p)
{
}
SystemCatalogNode::~SystemCatalogNode()
{
}
void SystemCatalogNode::start(QObject* subclass){
    _SYSTEM_CATALOG_IO_START(subclass);
}
void SystemCatalogNode::stop(QObject* subclass){
    _SYSTEM_CATALOG_IO_STOP(subclass);
}
bool SystemCatalogNode::readConnect(QObject* subclass,
                                    const SystemCatalogInput& properties, 
                                    const QDomElement& node, 
                                    const QDomElement& connect)
{
    _SYSTEM_CATALOG_IO_RC(subclass,properties,node,connect);
}
