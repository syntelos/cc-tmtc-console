/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <Qt>
#include <QAction>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "StorageTreeEditorDialog.h"

StorageTreeEditorDialog::StorageTreeEditorDialog(ObjectTreeModel* subject, QWidget* parent)
    : QDialog(parent), editor(new StorageTreeEditor(subject,this))
{
    setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(this->editor);

    layout->addSpacing(12);
    {
        QPushButton* close = new QPushButton("Close");

        close->setObjectName("closeButton");

        connect(close,SIGNAL(released()),this,SLOT(completed()));

        layout->addWidget(close);
    }
    layout->addStretch(1);

    setLayout(layout);
    /*
     * Actions
     */
    {
        QKeySequence ctrlA(Qt::CTRL + Qt::Key_A);
        QKeySequence ctrlD(Qt::CTRL + Qt::Key_D);
        QKeySequence ctrlS(Qt::CTRL + Qt::Key_S);

        QAction* add = new QAction("Add",this);
        add->setShortcut(ctrlA);

        QAction* del = new QAction("Delete",this);
        del->setShortcut(ctrlD);

        QAction* save = new QAction("Save",this);
        save->setShortcut(ctrlS);

        this->addAction(add);
        add->connect(add,SIGNAL(triggered()),editor,SLOT(addRecord()));

        this->addAction(del);
        del->connect(del,SIGNAL(triggered()),editor,SLOT(delRecord()));

        this->addAction(save);
        save->connect(save,SIGNAL(triggered()),editor,SLOT(saveAll()));
    }
    show();
}
StorageTreeEditorDialog::~StorageTreeEditorDialog(){
}
void StorageTreeEditorDialog::connectFinishedTo(const QObject* receiver, const char* slot){

    this->connect(this,SIGNAL(finished(int)),receiver,slot);
}
void StorageTreeEditorDialog::completed(){

    QDialog::done(0);
}
