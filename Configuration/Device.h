/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Device_H
#define _CONSOLE_CONFIGURATION_Device_H

#include <QMetaProperty>
#include <QObject>
#include <QScriptEngine>
#include <QUuid>
#include <QWidget>

#include "Multiplex/MultiplexTable.h"
#include "System/SystemDevice.h"
#include "System/SystemDeviceConnection.h"
#include "System/SystemDeviceIdentifier.h"
#include "ObjectTree/ObjectTreeNode.h"

class Device : public ObjectTreeNode, public SystemDevice {
    Q_OBJECT;
    Q_PROPERTY(QString* libraryUuid READ getLibraryUuid WRITE setLibraryUuid USER true FINAL);
    Q_PROPERTY(const SystemDeviceIdentifier& systemDeviceIdentifier READ getSystemDeviceIdentifier USER true FINAL);
    Q_CLASSINFO("Object Name", "systemDeviceIdentifier");

    QString* libraryUuid;

 public:
    static void InitScriptMetaType(QScriptEngine* engine);

    const SystemDeviceIdentifier& identifier;
    /*!
     * User constructor for emitting new values via database write.
     * 
     * This instance takes responsibility for the heap allocation of
     * the identifier (\a sid) instance object.
     */
    Device(const SystemDeviceIdentifier& sid, QObject* parent);
    /*!
     */
    ~Device();
    /*!
     */
    bool isInert();
    /*!
     * Reference to library by its own UUID.  A null value here
     * defaults to the builtin library.
     */
    QString* getLibraryUuid() const;
    /*!
     * The instance object (of this class) will take responsibility
     * for the heap allocation of the argument.
     */
    void setLibraryUuid(QString* libraryUuid);
    /*!
     * Copy the argument.
     */
    void setLibraryUuid(QString& libraryUuid);
    /*!
     */
    MultiplexTable* createMultiplexTable();
    /*!
     */
    MultiplexTable* findMultiplexTable() const;
    /*!
     */
    virtual const SystemDeviceIdentifier& getSystemDeviceIdentifier() const;
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
