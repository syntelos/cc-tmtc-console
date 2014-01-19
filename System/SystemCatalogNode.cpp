/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemCatalogNode.h"

void SystemCatalogNode::start(QObject* subclass){
    const QObjectList& children = subclass->children();
    const int count = children.count();
    int cc;
    for (cc = 0; cc < count; cc++){
        QObject* child = children.at(cc);
        SystemCatalogNode* node = dynamic_cast<SystemCatalogNode*>(child);
        if (node){
            node->start();
        }
    }
}
void SystemCatalogNode::stop(QObject* subclass){
    const QObjectList& children = subclass->children();
    const int count = children.count();
    int cc;
    for (cc = 0; cc < count; cc++){
        QObject* child = children.at(cc);
        SystemCatalogNode* node = dynamic_cast<SystemCatalogNode*>(child);
        if (node){
            node->stop();
        }
    }
}
