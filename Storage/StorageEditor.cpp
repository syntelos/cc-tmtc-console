/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <Qt>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaType>
#include <QObject>
/*
 * GUI
 */
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QTimeEdit>
#include <QVariant>
#include <QVBoxLayout>
/*
 * Local
 */
#include "StorageEditor.h"
#include "StorageTreeEditor.h"

StorageEditor::StorageEditor(QObject* subject, QWidget* parent)
    : QWidget(parent), formWidget(new StorageObjectForm(subject,this))
{
    init();
}
StorageEditor::StorageEditor(QObject* subject, StorageInterface* storage, QWidget* parent)
    : QWidget(parent), formWidget(new StorageObjectForm(subject,storage,this))
{
    init();
}
StorageEditor::~StorageEditor(){

    formWidget = vnul; // child destroyed by QObject
}
void StorageEditor::init(){
    /*
     * Initialize buttons style
     */
    if ( qobject_cast<QDialog*>(this->parent())){
        /*
         * When the parent of StorageEditor is a Dialog, as in the
         * case of StorageEditorDialog, ...
         */
        this->parentDialog = true;
    }
    else {
        /*
         * ... otherwise this component is part of a storage tree
         * editor
         */
        this->parentDialog = false;
    }
    /*
     * Initialize layout geometry
     */
    setMinimumSize(500,500);
    /*
     * Initialize buttons
     */
    QWidget* buttonsWidget = new QWidget();
    {
        QHBoxLayout* buttonsLayout = new QHBoxLayout();

        const bool isObjectTreeList = formWidget->isObjectTreeList();
        const bool isImportExport = formWidget->isObjectTreeImportExport();
        const bool hasUserProperties = formWidget->hasUserProperties();
        /*
         */
        if (isObjectTreeList){
            /*
             */
            StorageTreeEditor* parentStorageTreeEditor = dynamic_cast<StorageTreeEditor*>(this->parent());

            if (parentStorageTreeEditor){
                /*
                 * Button add
                 */
                QPushButton* add = new QPushButton("Add Record");
                add->setObjectName("addButton");
                //
                connect(add,SIGNAL(released()),parentStorageTreeEditor,SLOT(addRecord()));
                /*
                 * Button delete
                 */
                QPushButton* del = new QPushButton("Delete Record");
                del->setObjectName("deleteButton");
                //
                connect(del,SIGNAL(released()),parentStorageTreeEditor,SLOT(delRecord()));

                buttonsLayout->addWidget(add);
                buttonsLayout->addWidget(del);
            }
        }
        /*
         */
        if (isObjectTreeList || hasUserProperties){
            /*
             */
            if (parentDialog){
                /*
                 * Button accept
                 */
                QPushButton* accept = new QPushButton("Accept");
                accept->setObjectName("acceptButton");
                //
                connect(accept,SIGNAL(released()),formWidget,SLOT(accept()));
                /*
                 * Button cancel
                 */
                QPushButton* cancel = new QPushButton("Cancel");
                cancel->setObjectName("cancelButton");
                //
                connect(cancel,SIGNAL(released()),formWidget,SLOT(cancel()));

                buttonsLayout->addWidget(accept);
                buttonsLayout->addWidget(cancel);
            }
            else {
                /*
                 * Button update
                 */
                QPushButton* update = new QPushButton("Update");
                update->setObjectName("updateButton");
                //
                connect(update,SIGNAL(released()),formWidget,SLOT(accept()));

                buttonsLayout->addWidget(update);
            }
        }
        /*
         */
        if (isImportExport){
            /*
             * Button import
             */
            QPushButton* import = new QPushButton("Import");
            import->setObjectName("importButton");
            //
            connect(import,SIGNAL(released()),formWidget,SLOT(importNode()));

            buttonsLayout->addWidget(import);
            /*
             * Button export
             */
            QPushButton* exportButton = new QPushButton("Export");
            exportButton->setObjectName("exportButton");
            //
            connect(exportButton,SIGNAL(released()),formWidget,SLOT(exportNode()));

            buttonsLayout->addWidget(exportButton);
        }

        buttonsWidget->setLayout(buttonsLayout);
    }
    /*
     * Initialize layout
     */
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();

        mainLayout->addWidget(formWidget);

        mainLayout->addSpacing(12);

        mainLayout->addWidget(buttonsWidget);

        mainLayout->addStretch(1);

        setLayout(mainLayout);
    }
    /*
     */
    show();
}
void StorageEditor::connectFormCompletedTo(const QObject* receiver, const char* method){

    connect(formWidget,SIGNAL(completed(int)),receiver,method);
}
bool StorageEditor::hasUserProperties(){
    return formWidget->hasUserProperties();
}
int StorageEditor::getCountUserProperties(){
    return formWidget->getCountUserProperties();
}
bool StorageEditor::isSubject(QObject* node){
    return formWidget->isSubject(node);
}
void StorageEditor::accept(){

    formWidget->accept();
}
void StorageEditor::cancel(){

    formWidget->cancel();
}
