/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Device_H
#define _CONSOLE_CONFIGURATION_Device_H

#include <QMetaProperty>
#include <QObject>
#include <QSqlQuery>
#include <QUuid>
#include <QWidget>

#include "System/SystemDevice.h"
#include "System/SystemDeviceConnection.h"
#include "System/SystemDeviceIdentifier.h"
#include "Storage/StorageListItem.h"

class Device : public StorageListItem, public SystemDevice {
    Q_OBJECT;
    Q_PROPERTY(QString hostUuid READ getHostUuid USER false FINAL);
    Q_PROPERTY(QString libraryUuid READ getLibraryUuid WRITE setLibraryUuid USER true FINAL);
    Q_PROPERTY(QString connectionIdentifier READ getConnectionIdentifier WRITE setConnectionIdentifier USER true FINAL);
    Q_CLASSINFO("Object Name", "connectionIdentifier");

    QString* libraryUuid;
    QString* connectionIdentifier;
    /*!
     * Create table and populate with default values
     */
    void init();

 public:
    /*!
     * User constructor for emitting new values via database write.
     */
    Device(QObject* parent);
    /*!
     * Database read constructor
     */
    Device(QSqlQuery& query, int start, QObject* parent);
    /*!
     * Database read constructor
     */
    Device(QSqlQuery& query, QObject* parent);
    /*!
     */
    ~Device();
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
     * Read-only host UUID
     */
    const QString* getHostUuid() const;
    /*!
     * Reference to library by its own UUID.  A null value here
     * defaults to the builtin library.
     */
    const QString* getLibraryUuid() const;
    /*!
     */
    void setLibraryUuid(QString* libraryUuid);
    /*!
     */
    void setLibraryUuid(QString& libraryUuid);
    /*!
     * Connection identifier parsed for its components.
     * 
     * The caller is responsible for the returned heap allocation.
     */
    virtual const SystemDeviceIdentifier* getSystemDeviceIdentifier() const;
    /*!
     * Return an existing connection, may be null.
     */
    virtual const SystemDeviceConnection* getSystemDeviceConnection() const;
    /*!
     * Get or create a connection, may be null on a library failure.
     */
    const SystemDeviceConnection* createSystemDeviceConnection();

    void shutdownSystemDeviceConnection();
    /*!
     * Connection address/port identifier string
     */
    virtual const QString* getConnectionIdentifier() const;
    /*!
     */
    void setConnectionIdentifier(QString* connectionIdentifier);
    /*!
     */
    void setConnectionIdentifier(QString& connectionIdentifier);
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
    Q_DISABLE_COPY(Device)
};

Q_DECLARE_METATYPE(Device*)
#endif
