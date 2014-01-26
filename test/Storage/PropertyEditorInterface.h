/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_STORAGE_PropertyEditorInterface_H
#define _CONSOLE_STORAGE_PropertyEditorInterface_H

#include <QMetaProperty>
#include <QObject>
#include <QWidget>

/*!
 * This interface may be implemented by the storage editor subject as
 * a widget factory function.
 * 
 * Implementors need to return editors for user properties with
 * unusual types (not built-in types having widgets).  Otherwise the
 * definition of this method may return NULL for default widgets.
 * 
 * Implementors should return editors in order to capture the
 * opportunity to perform validation.
 * 
 * 
 * Use the class info property name "Object Name" with value (meta
 * object) property name to identify a property change that effects
 * the view (via \class QObject "set object name").
 */
class PropertyEditorInterface {

 public:
    /*!
     * Return an editor for this (user) property.  The implementor is
     * expected to be aware of read-only (user) properties.
     * 
     * This is used for "combo box" and "password line edit", for
     * example.  In the case of the "combo box", this package expects
     * the item set to be defined using data and text strings.
     */
    virtual QWidget* createPropertyFormEditor(int index, const QMetaProperty& property) = 0;
    /*!
     * Return a label for this (user) property
     */
    virtual QWidget* createPropertyFormLabel(int index, const QMetaProperty& property) = 0;
    /*!
     * Return TRUE for this method to be responsible to copy user
     * input from the editor into the implementor's data model.
     * 
     * Return FALSE to accept the caller to the copy user input from
     * the editor into the data model.
     * 
     * When the "create property form editor" method returns NULL, or
     * built in widgets like "spin box", "line edit", or "combo box",
     * the implementor may return FALSE for the default processing.
     */
    virtual bool setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor) = 0;

};

#endif
