/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_OBJECTTREE_ObjectTreeView_H
#define _CONSOLE_OBJECTTREE_ObjectTreeView_H

#include <QObject>
#include <QModelIndex>
#include <QTreeView>
#include <QWidget>

#include "ObjectTreeModel.h"

/*!
 * The constructor hides the default header for a plain tree view
 * component.
 */
class ObjectTreeView : public QTreeView {
    Q_OBJECT;

 public:
    /*!
     * 
     */
    class Selection {
    public:
        int child;
        QModelIndex parent;

        Selection(int child, const QModelIndex& parent)
            : child(child), parent(parent)
        {
        }
    };

 private:

    Selection* selection;

 public:
    /*!
     * This constructor initializes the view with "setModel" and
     * "show".
     */
    ObjectTreeView(ObjectTreeModel* model, QWidget* parent = 0);
    /*!
     */
    ~ObjectTreeView();
    /*!
     * Save selection parameters for post processing.
     */
    void beginSelection(int child, const QModelIndex& parent);
    /*!
     * Perform post processing of tree view selection.
     */
    void endSelection();

 public slots:
    /*!
     * View button add record
     */
    void addRecord();
    /*!
     * View button delete record
     */
    void delRecord();

 private:
    Q_DISABLE_COPY(ObjectTreeView)
};

#endif
