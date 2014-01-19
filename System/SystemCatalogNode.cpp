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
bool SystemCatalogNode::readConnect(QObject* subclass,
                                    const SystemCatalogInput& properties, 
                                    const QDomElement& node, 
                                    const QDomElement& connect)
{
    QString senderId = node.attribute("id");
    if (!senderId.isEmpty()){
        QString receiverId = connect.attribute("receiver");
        if (!receiverId.isEmpty()){
            QString signal = connect.attribute("signal");
            if (!signal.isEmpty()){
                QString slot = connect.attribute("slot");
                if (!slot.isEmpty()){

                    properties.sender(senderId,subclass,
                                      receiverId,
                                      signal,slot);

                    return true;
                }
            }
        }
    }
    return false;
}
