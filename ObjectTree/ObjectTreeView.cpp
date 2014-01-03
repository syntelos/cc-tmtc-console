/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QItemSelectionModel>

#include "ObjectTreeView.h"
#include "ObjectTreeList.h"

ObjectTreeView::ObjectTreeView(ObjectTreeModel* model, QWidget* parent)
    : QTreeView(parent)
{
    this->header()->hide();

    setUniformRowHeights(true);

    setMinimumSize(200,200);

    setModel(model);

    model->setParent(this);

    show();
}
ObjectTreeView::~ObjectTreeView(){

    QObject* model = this->model();
    model->setParent(vnul);
}
void ObjectTreeView::addRecord(){
    QModelIndex parent = this->selectionModel()->currentIndex();
    if (parent.isValid()){
        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(this->model());
        if (model){
            /*
             * Append, regardless of selection
             */
            model->addRecord(model->children().size(),parent);
        }
    }
}
void ObjectTreeView::delRecord(){
    QModelIndex selected = this->selectionModel()->currentIndex();
    if (selected.isValid()){
        ObjectTreeModel* model = static_cast<ObjectTreeModel*>(this->model());
        if (model){
            /*
             * Remove selected
             */
            QModelIndex selectedParent = selected.parent();

            int selectedRow = selected.row();

            model->delRecord(selectedRow,selectedParent);
        }
    }
}
void ObjectTreeView::beginSelection(int child, const QModelIndex& parent){

    this->selection = new ObjectTreeView::Selection(child,parent); //(copy to heap)

}
void ObjectTreeView::endSelection(){

    if (this->selection){

        ObjectTreeModel* model = dynamic_cast<ObjectTreeModel*>(this->model());

        if (-1 < this->selection->child){

            QModelIndex& parent = this->selection->parent;

            QModelIndex child = model->index(this->selection->child,0,parent);

            if (child.isValid()){
                /*
                 * Ensure that parent is expanded
                 */
                this->expand(parent);
                /*
                 * Select indexed child
                 */
                QItemSelectionModel* selection = this->selectionModel();

                selection->clearSelection();

                selection->select(child,QItemSelectionModel::Select);
            }
        }
        else {

            const QModelIndex& parent = this->selection->parent.parent();

            QObject* parent_object = static_cast<QObject*>(parent.internalPointer()); 

            QObject* child_object = static_cast<QObject*>(this->selection->parent.internalPointer());

            int child_index = parent_object->children().indexOf(child_object);

            QModelIndex child = model->index(child_index,0,parent);

            if (child.isValid()){
                /*
                 * Ensure that parent is expanded
                 */
                this->expand(parent);
                /*
                 * Select indexed child
                 */
                QItemSelectionModel* selection = this->selectionModel();

                selection->clearSelection();

                selection->select(child,QItemSelectionModel::Select);
            }
        }
        /*
         * Cleanup selection
         */
        delete this->selection;

        this->selection = vnul; //(delete)

    }
}
