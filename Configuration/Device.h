/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Device_H
#define _CONSOLE_CONFIGURATION_Device_H

#include <QMetaProperty>
#include <QObject>
#include <QScriptEngine>
#include <QWidget>

#include "Multiplex/Multiplex.h"
#include "Multiplex/MultiplexTable.h"
#include "System/SystemDevice.h"

/*!
 * This class may be extended via library or plugin.  The catalog DOM
 * attribute named "class" specifies the library or plugin identifier
 * (constructor argument) for \class SystemDeviceConstructorDiscovery.
 */
class Device : public SystemDevice {
    Q_OBJECT;
    Q_PROPERTY(QString* library READ getLibrary WRITE setLibrary USER true FINAL);
    Q_PROPERTY(const SystemDeviceIdentifier& systemDeviceIdentifier READ getSystemDeviceIdentifier USER true FINAL);
    Q_CLASSINFO("Object Name", "systemDeviceIdentifier");

    QString* library;

 public:
    static void InitScriptMetaType(QScriptEngine* engine);

    const SystemDeviceIdentifier& identifier;
    /*!
     * User constructor for emitting new values via database write.
     */
    Device(const SystemDeviceIdentifier& sid, QObject* parent);
    /*!
     */
    ~Device();
    /*!
     */
    bool isInert();
    /*!
     * Reference to library by its own .  A null value here
     * defaults to the builtin library.
     */
    QString* getLibrary() const;
    /*!
     * The instance object (of this class) will take responsibility
     * for the heap allocation of the argument.
     */
    void setLibrary(QString* library);
    /*!
     * Copy the argument.
     */
    void setLibrary(QString& library);
    /*!
     * Similar to stop, called from read.
     */
    void clear();
    /*!
     */
    Multiplex* getMultiplex() const;
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

 public slots:
    /*!
     * \sa SystemCatalogNode
     */
    virtual void start();
    /*!
     * \sa SystemCatalogNode
     */
    virtual void stop();
    /*!
     * \sa SystemCatalogNode
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&);
    /*!
     * \sa SystemCatalogNode
     */
    virtual void write(SystemCatalogOutput&, QDomElement&);

 private:
    Q_DISABLE_COPY(Device)
};

Q_DECLARE_METATYPE(Device*)
#endif
