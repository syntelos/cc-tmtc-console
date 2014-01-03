/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <Qt>

#include "StorageEditorDialog.h"

StorageEditorDialog::StorageEditorDialog(QObject* subject, QWidget* parent)
    : QDialog(parent), editor(new StorageEditor(subject,this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    /*
     */
    editor->connectFormCompletedTo(this,SLOT(done(int)));
}
StorageEditorDialog::StorageEditorDialog(QObject* subject, StorageInterface* storage, QWidget* parent)
    : QDialog(parent), editor(new StorageEditor(subject,storage,this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    /*
     */
    editor->connectFormCompletedTo(this,SLOT(done(int)));
}
StorageEditorDialog::~StorageEditorDialog(){

    editor = vnul;
}
void StorageEditorDialog::connectFinishedTo(const QObject* receiver, const char* slot){

    this->connect(this,SIGNAL(finished(int)),receiver,slot);
}
