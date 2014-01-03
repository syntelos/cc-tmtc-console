/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Library_H
#define _CONSOLE_CONFIGURATION_Library_H

#include <QMetaProperty>
#include <QObject>
#include <QSqlQuery>
#include <QUuid>
#include <QWidget>

#include "Storage/StorageListItem.h"
#include "Null.h"

/*!
 * Child of \class Libraries.
 */
class Library : public StorageListItem {
    Q_OBJECT;
    Q_PROPERTY(QString hostUuid READ getHostUuid USER false FINAL);
    Q_PROPERTY(QString libraryUuid READ getLibraryUuid USER false FINAL);
    Q_PROPERTY(QString fileIdentifier READ getFileIdentifier WRITE setFileIdentifier USER true FINAL);
    Q_PROPERTY(QString languageClassName READ getLanguageClassName USER true FINAL);
    Q_PROPERTY(QString connectionClassName READ getConnectionClassName USER true FINAL);
    Q_CLASSINFO("Object Name", "fileIndentifier");


    QString* libraryUuid;
    QString* fileIdentifier;
    QString* languageClassName;
    QString* connectionClassName;

 public:
    /*!
     * TODO (QLibrary&,QObject*)
     * 
     * User constructor for emitting new values via database write.
     * 
     * The parent argument is always \class Libraries.
     */
    Library(QObject* parent);
    /*!
     * Database read constructor.
     * 
     * The parent argument is always \class Libraries.
     */
    Library(QSqlQuery& query, int start, QObject* parent);
    /*!
     * Database read constructor.
     * 
     * The parent argument is always \class Libraries.
     */
    Library(QSqlQuery& query, QObject* parent);
    /*!
     */
    ~Library();
    /*!
     */
    virtual bool isInert();
    /*!
     * Retrieve bindings by index as for declared user properties.
     */
    virtual void read(QSqlQuery& query, int start = 0);
    /*!
     * Store bindings by index as for declared user properties.
     */
    virtual void write(QSqlQuery& query, int start = 0);
    /*!
     * Ref(Host UUID)
     */
    const QString* getHostUuid() const;
    /*!
     * Read-only library UUID
     */
    const QString* getLibraryUuid() const;
    /*!
     * Portable library loading identifier, see \class QLibrary.
     */
    const QString* getFileIdentifier() const;
    /*!
     */
    void setFileIdentifier(QString* fileIdentifier);
    /*!
     */
    void setFileIdentifier(QString& fileIdentifier);
    /*!
     * Read-only language class name
     */
    const QString* getLanguageClassName() const;
    /*!
     * Read-only connection class name
     */
    const QString* getConnectionClassName() const;

 private:
    /*!
     * Derived from library by inspection, stored in database as a
     * cache.
     */
    void setLibraryUuid(QString* libraryUuid);
    /*!
     */
    void setLibraryUuid(QString& libraryUuid);
    /*!
     */
    void setLanguageClassName(QString* languageClassName);
    /*!
     */
    void setLanguageClassName(QString& languageClassName);
    /*!
     */
    void setConnectionClassName(QString* connectionClassName);
    /*!
     */
    void setConnectionClassName(QString& connectionClassName);

 public:
    /*!
     */
    virtual QWidget* createPropertyFormEditor(int index, const QMetaProperty& property);
    /*!
     */
    virtual QWidget* createPropertyFormLabel(int index, const QMetaProperty& property);
    /*!
     */
    virtual bool setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor);

 private:
    Q_DISABLE_COPY(Library)
};

#endif
