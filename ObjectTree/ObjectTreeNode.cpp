/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "ObjectTreeNode.h"
#include "ObjectTreeModel.h"

/*
 * ObjectTreeNode
 */
ObjectTreeNode::ObjectTreeNode(QObject* parent)
    : QObject(parent), nodeView(vnul)
{
}
ObjectTreeNode::~ObjectTreeNode(){

    nodeView = vnul;
}
void ObjectTreeNode::beginInsertNode(int index1, int index2){
    ObjectTreeNodeAncestry* list = this->ancestry();
    if (list){
        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(list->node);

        if (model){
            /*
             * Append to list tail
             */
            (new ObjectTreeNodeAncestry(list,index1,index2));

            model->beginInsertNode(list);
        }

        delete list;
    }
}
void ObjectTreeNode::endInsertNode(){
    ObjectTreeNodeAncestry* list = this->ancestry();
    if (list){
        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(list->node);

        if (model){

            model->endInsertNode();
        }
        delete list;
    }
}
void ObjectTreeNode::beginRemoveNode(int index1, int index2){
    ObjectTreeNodeAncestry* list = this->ancestry();
    if (list){
        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(list->node);

        if (model){

            if (-1 < index1 && index1 <= index2){
                const QObjectList& children = this->children();

                if (index2 < children.size()){
                    /*
                     * Normal case
                     */
                    QObject* child = children.at(index1);
                    /*
                     * Append to list tail
                     */
                    (new ObjectTreeNodeAncestry(list,index1,index2,child));

                    model->beginRemoveNode(list);
                }
            }
        }
        delete list;
    }
}
void ObjectTreeNode::endRemoveNode(){
    ObjectTreeNodeAncestry* list = this->ancestry();
    if (list){
        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(list->node);

        if (model){

            model->endRemoveNode();
        }
        delete list;
    }
}
void ObjectTreeNode::nodeDataChanged(const char* propname){
    /*
     */
    ObjectNodeView* view = this->nodeView;
    if (view){

        qDebug() << "ObjectTreeNode nodeDataChanged [node view] (" << propname << ")";

        view->nodeDataChanged(propname);
    }
    /*
     */
    ObjectTreeNodeAncestry* list = this->ancestry();
    if (list){

        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(list->node);

        if (model){

            qDebug() << "ObjectTreeNode nodeDataChanged [model view] ";

            model->nodeDataChanged(list);
        }
        else {
            qDebug() << "ObjectTreeNode nodeDataChanged [missing model view]";
        }
    }
    else {
        qDebug() << "ObjectTreeNode nodeDataChanged [missing model view ancestry]";
    }
}
QWidget* ObjectTreeNode::ancestorWidget(){
    QObject* p = this->parent();
    while (p){
        QWidget* wp = dynamic_cast<QWidget*>(p);
        if (wp){

            return wp;
        }
        else {
            p = p->parent();
        }
    }
    return vnul;
}
ObjectTreeNodeAncestry* ObjectTreeNode::ancestry(){
    ObjectTreeNodeAncestry* list = vnul;
    int x;
    /*
     * Build list from tail (this)
     */
    QObject* c = this;
    QObject* p = c->parent();
    while (p){

        x = p->children().indexOf(c);

        list = new ObjectTreeNodeAncestry(c,x,list);

        if (dynamic_cast<ObjectTreeModel*>(p)){

            return new ObjectTreeNodeAncestry(p,0,list);
        }
        else {
            c = p;
            p = p->parent();
        }
    }

    if (list){

        delete list;
    }

    return vnul;
}
ObjectNodeView* ObjectTreeNode::getObjectNodeView(){

    return nodeView;
}
void ObjectTreeNode::setObjectNodeView(ObjectNodeView* view){

    this->nodeView = view;
}
/*
 * ObjectTreeNodeAncestry HEAD constructor builds a list from tail to head
 */
ObjectTreeNodeAncestry::ObjectTreeNodeAncestry(QObject* node, int index, ObjectTreeNodeAncestry* tail)
    : node(node), index1(index), index2(index), child(tail)
{
}
/*
 * ObjectTreeNodeAncestry TAIL constructor builds a list from head to tail
 */
ObjectTreeNodeAncestry::ObjectTreeNodeAncestry(ObjectTreeNodeAncestry* head, int index1, int index2, QObject* node)
    : node(node), index1(index1), index2(index2), child(vnul)
{
    ObjectTreeNodeAncestry* p = head;
    while (p){

        if (p->child){

            p = p->child;
        }
        else {

            p->child = this;
            break;
        }
    }
}
ObjectTreeNodeAncestry::~ObjectTreeNodeAncestry(){

    if (child){

        delete child;
    }
    node = vnul;
}
ObjectTreeNodeAncestry* ObjectTreeNodeAncestry::parent(){
    /*
     * Tail := {child: null}
     * Parent of Tail := {child: child: null}
     */
    if (child){

        if (child->child){

            return child->parent();
        }
        else {

            return this;
        }
    }
    else {

        return vnul;
    }
}
ObjectTreeNodeAncestry* ObjectTreeNodeAncestry::tail(){

    if (child){

        return child->tail();
    }
    else {

        return this;
    }
}
