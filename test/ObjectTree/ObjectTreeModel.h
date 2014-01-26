/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_OBJECTTREE_ObjectTreeModel_H
#define _CONSOLE_OBJECTTREE_ObjectTreeModel_H

#include <Qt>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <QVariant>

#include "ObjectTreeNode.h"

/*!
 * This class presents a hierarchical model interface to the \class
 * QObject tree rooted in the subclass node.
 * 
 * A subclass instance of this class is the root of a \class QObject
 * tree.  This tree has one root node, exclusively.
 * 
 * This class is applied to the Console Configuration tree user
 * interface.  The storage tree model has a hierarchical view for
 * navigating and managing the structure of its nodes, and the storage
 * editor view for creating and modifying the data content of its
 * nodes.
 * 
 * The name string for each tree view item is defined as the \class
 * QObject object name, or class name.  Members of the object tree
 * should define a simple and reasonable object name when their class
 * name will not be unique in the tree.
 */
class ObjectTreeModel : public QAbstractItemModel {

 public:
    /*!
     */
    ObjectTreeModel(QObject* parent = 0);
    /*!
     */
    ~ObjectTreeModel();
    /*!
     * Override superclass method for the GUI text string
     * representation of a node.
     */
    QVariant data(const QModelIndex& index, int role) const;
    /*!
     * String representation of node.
     * 
     * Returns object name when not empty, otherwise class name.
     * Nodes that are members of (parent) lists should define object
     * names.
     */
    QVariant nodeData(const QObject* object) const;
    /*!
     * Override superclass method to return "selectable" for any
     * indexable node.
     */
    Qt::ItemFlags flags(const QModelIndex& index) const;
    /*!
     * Override superclass method to returns null variant.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    /*!
     * Override superclass method to return any indexable node.
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    /*!
     * Override superclass method to return any indexable parent of
     * the referenced node.
     */
    QModelIndex parent(const QModelIndex& index) const;
    /*!
     * Override superclass method to return whether the referenced
     * node has children.
     */
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const;
    /*!
     * Override superclass method to return the number of children of
     * referenced node.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    /*!
     * Override superclass method, always returns 1.
     */
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    /*!
     * Override superclass method, always returns false.
     */
    bool insertColumns(int index, int many, const QModelIndex& parent = QModelIndex());
    /*!
     * Override superclass method, always returns false.
     */
    bool removeColumns(int index, int many, const QModelIndex& parent = QModelIndex());
    /*!
     * Override superclass method, ignoring \a many
     */
    bool insertRows(int index, int many, const QModelIndex& parent = QModelIndex());
    /*!
     * Override superclass method, ignoring \a many
     */
    bool removeRows(int index, int many, const QModelIndex& parent = QModelIndex());

    void addRecord(int index, const QModelIndex& parent);

    void delRecord(int index, const QModelIndex& parent);
    /*!
     * Descendant access to object tree model.  The column index is
     * always zero.  The tail index is always zero, and the tail node
     * is always null.
     */
    void beginInsertNode(ObjectTreeNodeAncestry* ancestry);
    /*!
     * Descendant access to object tree model.
     */
    void endInsertNode();
    /*!
     * Descendant access to object tree model.  The column index is
     * always zero.  The tail index and node are zero-positive and
     * non-null (respectively).
     */
    void beginRemoveNode(ObjectTreeNodeAncestry* ancestry);
    /*!
     * Descendant access to object tree model.
     */
    void endRemoveNode();
    /*!
     * Repaint view
     */
    void nodeDataChanged(ObjectTreeNodeAncestry* node);
};

#endif
