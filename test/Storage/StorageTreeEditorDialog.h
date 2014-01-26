/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageTreeEditorDialog_H
#define _CONSOLE_STORAGE_StorageTreeEditorDialog_H

#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QWidget>
#include <QDialog>

#include "ObjectTree/ObjectTreeModel.h"
#include "StorageTreeEditor.h"

/*!
 * An object tree editor dialog.
 */
class StorageTreeEditorDialog : public QDialog
{
    Q_OBJECT;

    StorageTreeEditor* editor;

 public:
    StorageTreeEditorDialog(ObjectTreeModel* subject, QWidget* parent = 0);

    ~StorageTreeEditorDialog();

    void connectFinishedTo(const QObject* receiver, const char* slot);

 public slots:

    void completed();

};

#endif
