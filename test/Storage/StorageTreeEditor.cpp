/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDialog>
#include <QObject>

#include "StorageTreeEditor.h"
#include "StorageEditor.h"
#include "StorageInterface.h"
#include "PropertyEditorInterface.h"
#include "ObjectTree/ObjectTreeView.h"

StorageTreeEditor::StorageTreeEditor(ObjectTreeModel* model, QWidget* parent)
    : QSplitter(parent), treeModel(model), treeView(new ObjectTreeView(model,this)), nodeEditor(new StorageEditor(model,this))
{
    this->addWidget(treeView);
    this->addWidget(nodeEditor);
    /*
     * "activated" doesn't work on linux
     */
    treeView->connect(treeView,SIGNAL(activated(const QModelIndex&)),this,SLOT(selected(const QModelIndex&)));
    treeView->connect(treeView,SIGNAL(clicked(const QModelIndex&)),this,SLOT(selected(const QModelIndex&)));
    treeView->connect(treeView,SIGNAL(entered(const QModelIndex&)),this,SLOT(selected(const QModelIndex&)));

    setMinimumSize(700,500);

    show();
}
StorageTreeEditor::~StorageTreeEditor(){
}
void StorageTreeEditor::selected(const QModelIndex& index){
    if (index.isValid()){

        QObject* node = static_cast<QObject*>(index.internalPointer());

        if (!nodeEditor->isSubject(node)){
            /*
             * Change editor, delete old
             */
            nodeEditor->setParent(0);
            nodeEditor->deleteLater();
            /*
             * Choose StorageEditor constructor
             */
            if (dynamic_cast<PropertyEditorInterface*>(node)){

                if (dynamic_cast<StorageInterface*>(node)){

                    nodeEditor = new StorageEditor(node,this);
                }
                else {
                    StorageInterface* storage = 0;
                    QObject* p = node->parent();
                    while (true){
                        storage = dynamic_cast<StorageInterface*>(p);

                        if (storage){

                            break;
                        }
                        else {
                            p = p->parent();
                            if (!p){

                                break;
                            }
                        }
                    }

                    nodeEditor = new StorageEditor(node,storage,this);
                }
            }
            else {

                nodeEditor = new StorageEditor(node,this);
            }
            this->addWidget(nodeEditor);

            this->show();
        }
    }
}
void StorageTreeEditor::addRecord(){

    treeView->addRecord();
}
void StorageTreeEditor::delRecord(){

    treeView->delRecord();
}
void StorageTreeEditor::saveAll(){

    nodeEditor->accept();
}
