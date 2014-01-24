/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_OBJECTTREE_ObjectTreeList_H
#define _CONSOLE_OBJECTTREE_ObjectTreeList_H

/*!
 * A node in a \class QObject tree implements this interface if it is
 * capable of adding and removing children.
 */
class ObjectTreeList {

 public:
    /*!
     * Edit node, appending a new child: return success.
     */
    virtual bool insertObjectTreeList() = 0;
    /*!
     * Edit node, removing the indexed child: return success.
     */
    virtual bool removeObjectTreeList(int idx) = 0;

    // bool Devices::insertObjectTreeList(){
    //     const QObjectList& children = this->children();
    //     const int index = children.size();
    //     if (0 == index){
    // 
    //         beginInsertNode(index,index);
    // 
    //         //(new Device(this));
    // 
    //         endInsertNode();
    // 
    //         return true;
    //     }
    //     else {
    // 
    //         beginInsertNode(index,index);
    // 
    //         //(new Device(this));
    // 
    //         endInsertNode();
    // 
    //         return true;
    //     }
    // }
    // bool Devices::removeObjectTreeList(int idx){
    //     if (-1 < idx){
    //         QObjectList& children = const_cast<QObjectList&>(this->children());
    //         if (idx < children.size()){
    // 
    //             beginInsertNode(idx,idx);
    // 
    //             delete children.takeAt(idx);
    // 
    //             endInsertNode();
    // 
    //             return true;
    //         }
    //     }
    //     return false;
    // }

};

#endif
