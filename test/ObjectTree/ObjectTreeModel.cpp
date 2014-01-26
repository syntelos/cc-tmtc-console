/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QList>
#include <QItemSelectionModel>
#include <QObjectList>

#include "ObjectTreeList.h"
#include "ObjectTreeModel.h"
#include "ObjectTreeView.h"

ObjectTreeModel::ObjectTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
}
ObjectTreeModel::~ObjectTreeModel(){
}
QVariant ObjectTreeModel::nodeData(const QObject* node) const {
    if (node){
        QString name = node->objectName();
        if (0 < name.length()){

            return name;
        }
        else {
            QString className = node->metaObject()->className();

            return className;
        }
    }
    else
        return QVariant();
}
QVariant ObjectTreeModel::data(const QModelIndex& index, int role) const {

    if (Qt::DisplayRole == role){

        const int row = index.row();
        /*
         * Init case for "parent of this"
         */
        if (-1 == row){

            return QVariant();
        }
        /*
         * {R,C} coordinates relative to node
         */
        else if (index.isValid()){

            QObject* node = static_cast<QObject*>(index.internalPointer());

            return this->nodeData(node);
        }
        /*
         * {R,C} coordinates relative to root
         */
        else if (0 == row){

            return this->nodeData(this);
        }
    }
    return QVariant();
}
Qt::ItemFlags ObjectTreeModel::flags(const QModelIndex& index) const {
    const int row = index.row();

    if (-1 < row){

        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    else {

        return 0;
    }
}
QVariant ObjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex& parent) const 
{
    /*
     * {R,C} coordinates relative to node
     */
    if (parent.isValid()){

        QObject* parent_node = static_cast<QObject*>(parent.internalPointer());

        const QObjectList& children = parent_node->children();

        if (row < children.size()){

            QObject* child = children.value(row);

            return createIndex(row,column,child);
        }
    }
    /*
     * {R,C} coordinates relative to root
     */
    else if (0 == row){

        ObjectTreeModel* root = const_cast<ObjectTreeModel*>(this);

        return createIndex(row,column,root);
    }

    return QModelIndex();
}
QModelIndex ObjectTreeModel::parent(const QModelIndex &index) const {

    const int row = index.row();

    if (index.isValid()){

        QObject* node = static_cast<QObject*>(index.internalPointer());
        /*
         * Need to index {R,C} the parent of the indexed node
         *
         * <this> has no parent
         */
        if (node != this){

            QObject* node_parent = node->parent();

            if (node_parent != this){
                /*
                 * The parent of the parent node has the index of the
                 * parent node
                 */
                QObject* node_grandparent = node_parent->parent();

                QObjectList node_parent_siblings = node_grandparent->children();

                int node_parent_row = node_parent_siblings.indexOf(node_parent);

                return createIndex(node_parent_row,0,node_parent);
            }
            else {
                /*
                 * Parent is <this>
                 */
                return createIndex(0,0,node_parent);
            }
        }
    }

    return QModelIndex();
}
bool ObjectTreeModel::hasChildren (const QModelIndex& parent) const {

    const int row = parent.row();
    /*
     * Init case for "parent of this"
     */
    if (-1 == row){

        return true;
    }
    /*
     * {R,C} coordinates relative to node
     */
    else if (parent.isValid()){

        QObject* parent_node = static_cast<QObject*>(parent.internalPointer());

        QObjectList children = parent_node->children();

        return (0 < children.size());
    }
    /*
     * {R,C} coordinates relative to root
     */
    else if (0 == row){

        QObjectList children = this->children();

        return (0 < children.size());
    }
    else {
        return false;
    }
}
int ObjectTreeModel::rowCount(const QModelIndex& parent) const {

    const int row = parent.row();
    /*
     * Init case for "parent of this"
     */
    if (-1 == row){

        return 1;
    }
    /*
     * {R,C} coordinates relative to node
     */
    else if (parent.isValid()){

        QObject* parent_node = static_cast<QObject*>(parent.internalPointer());

        QObjectList children = parent_node->children();

        return children.size();
    }
    /*
     * {R,C} coordinates relative to root
     */
    else if (0 == row){

        QObjectList children = this->children();

        return children.size();
    }
    else {
        return 0;
    }
}
int ObjectTreeModel::columnCount(const QModelIndex& parent) const {

    return 1;
}
bool ObjectTreeModel::insertColumns(int index, int many, const QModelIndex& parent){

    return false;
}
bool ObjectTreeModel::removeColumns(int index, int many, const QModelIndex& parent){

    return false;
}
bool ObjectTreeModel::insertRows(int index, int many, const QModelIndex& parent){

    if (0 < many && -1 < index && parent.isValid()){
        QObject* parent_object = static_cast<QObject*>(parent.internalPointer());
        ObjectTreeList* parent_node = dynamic_cast<ObjectTreeList*>(parent_object);
        if (parent_node){
            index = parent_object->children().size();

            ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());
            if (view){

                view->beginSelection(index,parent);

                return true;
            }
        }
    }
    return false;
}
bool ObjectTreeModel::removeRows(int index, int many, const QModelIndex& parent){

    if (0 < many && -1 < index && parent.isValid()){
        ObjectTreeList* parent_node = dynamic_cast<ObjectTreeList*>(static_cast<QObject*>(parent.internalPointer()));
        if (parent_node){
            ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());
            if (view){

                view->beginSelection(index,parent);

                return true;
            }
        }
    }
    return false;
}
void ObjectTreeModel::beginInsertNode(ObjectTreeNodeAncestry* ancestry){
    if (ancestry){
        ObjectTreeNodeAncestry* p = ancestry->parent();
        if (p){
            ObjectTreeNodeAncestry* c = p->child;

            const QModelIndex& parent = createIndex(p->index1,0,p->node);

            beginInsertRows(parent,c->index1,c->index2);

            /*
             * Define tree view selection
             */
            ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());

            if (view){

                view->beginSelection(c->index1,parent);
            }
        }
    }
}
void ObjectTreeModel::endInsertNode(){

    endInsertRows();
    /*
     * Perform tree view selection
     */
    ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());

    if (view){

        view->endSelection();
    }
}
void ObjectTreeModel::beginRemoveNode(ObjectTreeNodeAncestry* ancestry){
    if (ancestry){
        ObjectTreeNodeAncestry* p = ancestry->parent();
        if (p){
            ObjectTreeNodeAncestry* c = p->child;
            /*
             *
             */
            const QModelIndex& parent = createIndex(p->index1,0,p->node);

            beginRemoveRows(parent,c->index1,c->index2);

            /*
             * Define tree view selection
             */
            ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());

            if (view){

                view->beginSelection(c->index1-1,parent);
            }
        }
    }
}
void ObjectTreeModel::endRemoveNode(){

    endRemoveRows();
    /*
     * Perform tree view selection
     */
    ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());

    if (view){

        view->endSelection();
    }
}
void ObjectTreeModel::addRecord(int index, const QModelIndex& parent){
    QObject* parent_object = static_cast<QObject*>(parent.internalPointer());
    ObjectTreeList* parent_node = dynamic_cast<ObjectTreeList*>(parent_object);
    if (parent_node){
        parent_node->insertObjectTreeList();
    }
}
void ObjectTreeModel::delRecord(int index, const QModelIndex& parent){
    ObjectTreeList* parent_node = dynamic_cast<ObjectTreeList*>(static_cast<QObject*>(parent.internalPointer()));
    if (parent_node){
        parent_node->removeObjectTreeList(index);
    }
}
void ObjectTreeModel::nodeDataChanged(ObjectTreeNodeAncestry* node){
    if (node){
        ObjectTreeView* view = dynamic_cast<ObjectTreeView*>(this->QObject::parent());
        if (view){

            ObjectTreeNodeAncestry* p = node->parent();
            if (p){
                ObjectTreeNodeAncestry* c = p->child;
                if (c){

                    const QModelIndex& child = createIndex(c->index1,0,c->node);

                    qDebug() << "ObjectTreeModel nodeDataChanged [" << nodeData(c->node) << "]";

                    view->dataChanged(child,child);
                }
                else {
                    qDebug() << "ObjectTreeModel nodeDataChanged [missing ancestry child]";
                }
            }
            else {
                qDebug() << "ObjectTreeModel nodeDataChanged [missing ancestry parent]";
            }
        }
        else {
            qDebug() << "ObjectTreeModel nodeDataChanged [missing view]";
        }
    }
    else {
        qDebug() << "ObjectTreeModel nodeDataChanged [missing ancestry]";
    }
}
