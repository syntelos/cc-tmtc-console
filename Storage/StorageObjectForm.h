/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageObjectForm_H
#define _CONSOLE_STORAGE_StorageObjectForm_H

#include <QMetaObject>
#include <QVariant>
#include <QWidget>

#include "StorageError.h"
#include "StorageInterface.h"
#include "PropertyEditorInterface.h"
#include "ObjectTree/ObjectNodeView.h"

/*!
 * Editing form separates the subject from the property editor for
 * objects and lists.
 */
class StorageObjectForm : public QWidget, public ObjectNodeView {
    Q_OBJECT;

    QObject* subject;
    StorageInterface* storageInterface;
    PropertyEditorInterface* propertyEditor;
    int countUserProperties;

    void init();

 public:
    /*!
     * Any \class StorageInterface and \class PropertyEditorInterface
     * must be implemented by \a subject
     */
    StorageObjectForm(QObject* subject, QWidget* parent);
    /*!
     * Any \class PropertyEditorInterface must be implemented by \a
     * subject
     */
    StorageObjectForm(QObject* subject, StorageInterface* storage, QWidget* parent);
    /*!
     */
    ~StorageObjectForm();
    /*!
     * Create one of the default edit widgets for recognized subtypes
     * of \class QVariant.
     */
    QWidget* createEditor(QVariant& value);
    /*!
     * \class QObject node (subject) has user properties.
     */
    bool hasUserProperties();
    /*!
     * Number of node user properties.
     */
    int getCountUserProperties();
    /*!
     * The \class QObject node (subject) is identical to the argument
     * \a node.
     */
    bool isSubject(QObject* node);
    /*!
     * The \class QObject node (subject) is a member of the class
     * \class ObjectTreeList.
     */
    bool isObjectTreeList();
    /*!
     * The \class QObject node (subject) is a member of the class
     * \class ObjectTreeImportExport.
     */
    bool isObjectTreeImportExport();
    /*!
     * Return the value of class info property named "Object Name".
     * This class info property names the object property which sets
     * the object name.
     */
    const char* identityProperty(const QMetaObject* meta);
    /*!
     * Update the content of the named field
     */
    virtual void nodeDataChanged(const char* propname);

 signals:
    /*!
     * Scene graph output (QDialog::done(int)).  Emits 0 for accepted,
     * 1 for cancelled.
     */
    void completed(int);

 public slots:
    /*!
     * User input store
     */
    void accept();
    /*!
     * User input discard
     */
    void cancel();
    /*!
     * User input for "node import"
     */
    void importNode();
    /*!
     * User input for "node export"
     */
    void exportNode();

 private:
    Q_DISABLE_COPY(StorageObjectForm)
};

#endif
