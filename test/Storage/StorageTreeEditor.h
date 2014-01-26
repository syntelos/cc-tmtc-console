/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageTreeEditor_H
#define _CONSOLE_STORAGE_StorageTreeEditor_H

#include <QModelIndex>
#include <QScriptable>
#include <QSplitter>
#include <QWidget>

#include "ObjectTree/ObjectTreeModel.h"
#include "ObjectTree/ObjectTreeView.h"
#include "StorageEditor.h"

/*!
 * Split pane shows the tree view on the left, and the editor for the
 * selected component on the right.
 * 
 * Navigating the tree view changes the editor to the currently
 * selected object tree node.
 */
class StorageTreeEditor : public QSplitter,
    protected QScriptable
{
    Q_OBJECT;

    ObjectTreeModel* treeModel;
    ObjectTreeView* treeView;
    StorageEditor* nodeEditor;

 public:
    /*!
     * \a Model is the root of the \class QObject tree for editing.
     * This class creates a \class ObjectTreeView for this model, and
     * manages \class StorageEditor instances for the navigation of
     * the object tree.
     * 
     * The "activated" signal from the \class ObjectTreeView (\class
     * QAbstractItemView) is connected to the "selected" slot in this
     * class for navigation.
     */
    StorageTreeEditor(ObjectTreeModel* model, QWidget* parent = 0);
    ~StorageTreeEditor();

 public slots:
    void selected(const QModelIndex& index);

    void addRecord();

    void delRecord();

    void saveAll();

 private:
    Q_DISABLE_COPY(StorageTreeEditor)
};

#endif
