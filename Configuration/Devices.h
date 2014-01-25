/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Devices_H
#define _CONSOLE_CONFIGURATION_Devices_H

#include <QObject>

#include "System/SystemMultiplex.h"
#include "System/SystemMultiplexTable.h"
#include "ObjectTree/ObjectTreeNode.h"
#include "ObjectTree/ObjectTreeList.h"
#include "System/SystemCatalogNode.h"
#include "Device.h"

class Devices : public SystemMultiplex
{
    Q_OBJECT;


 public:
    static void InitScriptMetaType(QScriptEngine* engine);
    /*!
     */
    Devices(QObject* parent = 0);
    /*!
     */
    ~Devices();
    /*!
     * Update the time series data set.  The caller is responsible for
     * the heap allocation of the arguments, which are copied by this
     * method.
     */
    virtual bool update(const SystemMessage*);
    /*!
     * Query the time series data set.  The caller is responsible for
     * the returned heap allocation.
     */
    virtual SystemMessage* query(const SystemMessage*);
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    virtual QVariant query(const SystemDeviceIdentifier&, const SystemName&);
    /*!
     * Plot selector builds the requested set of path structures with
     * visualization into the requested window.
     */
    virtual void select(int count, SystemMultiplexSelect** query, const QRectF& window);

    QList<Device*> listDevices();

    Device* findDevice(const SystemDeviceIdentifier& sid) const;

    SystemMultiplexTable* findSystemMultiplexTable(const SystemDeviceIdentifier& sid) const;

    QList<SystemDeviceIdentifier> findSystemMultiplexTableIdentifiers();

 signals:
    /*!
     * The database emits this signal to transmit to a device
     * connection.
     */
    void sendToDevice(const SystemMessage*);
    /*!
     * The database emits this signal to transmit to a user interface
     * (e.g. Terminal Output)
     */
    void sendToUser(const SystemMessage*);

 public slots:
    /*!
     */
    void clear();
    /*!
     */
    virtual void start();
    /*!
     */
    virtual void stop();
    /*!
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&);
    /*!
     */
    virtual void write(SystemCatalogOutput&, QDomElement&);
    /*!
     * The database receives this signal from a device connection to
     * update the time series.
     */
    virtual void receivedFromDevice(const SystemMessage*);
    /*!
     * The database receives this signal from a user interface
     * (e.g. Terminal Input) to query the time series.  
     * 
     * The identifier is assumed to have persistent allocation, and
     * will be employed in the response signal sent to users.
     */
    virtual void receivedFromUser(const SystemMessage*);

 private:
    Q_DISABLE_COPY(Devices)
};

Q_DECLARE_METATYPE(Devices*)
#endif
