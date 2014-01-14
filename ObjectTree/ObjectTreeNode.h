/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_OBJECTTREE_ObjectTreeNode_H
#define _CONSOLE_OBJECTTREE_ObjectTreeNode_H

#include <QObject>
#include <QModelIndex>
#include <QWidget>


#include "ObjectNodeView.h"

class ObjectTreeNodeAncestry;

/*!
 * A \class QObject tree descendent of an instance of \class
 * ObjectTreeModel.
 */
class ObjectTreeNode : public QObject {

    ObjectNodeView* nodeView;

 public:
    /*!
     */
    ObjectTreeNode(QObject* parent = 0);
    /*!
     */
    ~ObjectTreeNode();
    /*!
     */
    ObjectNodeView* getObjectNodeView();
    /*!
     */
    void setObjectNodeView(ObjectNodeView* nodeView);

 protected:
    /*!
     * Subclass access to object tree model.  This must be called for
     * a parent inserting a new child into its \class QObject tree.
     * 
     * The \a index of the child into the parent would be the number
     * of children in the parent.
     * 
     * This method produces an ancestry list for the \class
     * ObjectTreeModel that has a null tail node.
     */
    void beginInsertNode(int index1, int index2);
    /*!
     * Subclass access to object tree model
     */
    void endInsertNode();
    /*!
     * Subclass access to object tree model.  This must be called for
     * a parent removing a child from its \class QObject tree.
     * 
     * The \a index of the child into the parent would be the number
     * of children in the parent.
     * 
     * This method produces an ancestry list for the \class
     * ObjectTreeModel that has a non-null tail node.
     */
    void beginRemoveNode(int index1, int index2);
    /*!
     * Subclass access to object tree model
     */
    void endRemoveNode();
    /*!
     * Return the \class ObjectTreeModel node, with the \class QObject tree
     * path to this node.  The caller is responsible for deleting this
     * list from the returned pointer.
     * \sa \class ObjectTreeNodeAncestry
     */
    ObjectTreeNodeAncestry* ancestry();
    /*!
     * Update node view and model view
     */
    void nodeDataChanged(const char* propname);
    /*!
     */
    QWidget* ancestorWidget();

 private:
    Q_DISABLE_COPY(ObjectTreeNode);
};

/*!
 * A path through a \class QObject scene graph from \class
 * ObjectTreeModel to the \class ObjectTreeNode (inclusive).
 */
class ObjectTreeNodeAncestry {

 public:
    /*!
     * Tree node may be null when index is zero.
     */
    QObject* node;
    /*!
     * Index of node in its \class QObject parent.  The \class
     * ObjectTreeModel element of this list has staticly defined index
     * zero.
     */
    int index1;
    int index2;
    /*!
     * Ancestry list child
     */
    ObjectTreeNodeAncestry* child;

    /*!
     * List head constructor 
     */
    ObjectTreeNodeAncestry(QObject* node, int index, ObjectTreeNodeAncestry* tail = 0);
    /*!
     * List tail constructor
     */
    ObjectTreeNodeAncestry(ObjectTreeNodeAncestry* head, int index1, int index2, QObject* node = 0);
    /*!
     * Delete from top (to bottom, parent to child), as returned by
     * ObjectTreeNode::ancestry.
     */
    ~ObjectTreeNodeAncestry();
    /*!
     * Object tree node tail of list
     */
    ObjectTreeNodeAncestry* tail();
    /*!
     * Parent of tail
     */
    ObjectTreeNodeAncestry* parent();

 protected:

    void nodeDataChanged();

 private:
    Q_DISABLE_COPY(ObjectTreeNodeAncestry);
};

#endif
