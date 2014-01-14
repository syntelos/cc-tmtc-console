/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_StorageEditor_H
#define _CONSOLE_STORAGE_StorageEditor_H

#include <QWidget>

#include "StorageInterface.h"
#include "StorageObjectForm.h"

/*!
 * This class is a widget using the \class StorageObjectForm to edit a
 * node in a \class QObject tree.  It combines the form widget with
 * buttons, and connects buttons with the \class StorageObjectForm
 * slots "accept" and "cancel".
 */
class StorageEditor : public QWidget {
    Q_OBJECT;

    StorageObjectForm* formWidget;

    bool parentDialog;

    void init();

 public:

    /*!
     * The \a subject implements both \class StorageInterface and
     * \class PropertyEditorInterface if it has USER properties.
     */
    StorageEditor(QObject* subject, QWidget* parent);
    /*!
     * The \a subject implements \class PropertyEditorInterface if it
     * has USER properties.
     */
    StorageEditor(QObject* subject, StorageInterface* storage, QWidget* parent);
    /*!
     * 
     */
    ~StorageEditor();
    /*!
     * 
     */
    bool hasUserProperties();
    /*!
     * 
     */
    int getCountUserProperties();
    /*!
     * 
     */
    void connectFormCompletedTo(const QObject* receiver, const char* method);
    /*!
     * 
     */
    bool isSubject(QObject* node);

 public slots:
   /*!
    * Calls \class StorageObjectForm accept.
    */
   void accept();
   /*!
    * Calls \class StorageObjectForm cancel.
    */
   void cancel();

 private:

    Q_DISABLE_COPY(StorageEditor)
};

#endif
