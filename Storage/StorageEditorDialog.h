/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageEditorDialog_H
#define _CONSOLE_STORAGE_StorageEditorDialog_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include "StorageEditor.h"
#include "StorageInterface.h"

/*!
 * A single node editor dialog.
 */
class StorageEditorDialog : public QDialog {
    Q_OBJECT

 protected:
    StorageEditor* editor;

 public:
    StorageEditorDialog(QObject* subject, QWidget* parent = 0);
    StorageEditorDialog(QObject* subject, StorageInterface* storage, QWidget* parent = 0);
    ~StorageEditorDialog();

    void connectFinishedTo(const QObject* receiver, const char* slot);

};

#endif
