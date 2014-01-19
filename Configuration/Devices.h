/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Devices_H
#define _CONSOLE_CONFIGURATION_Devices_H

#include <QObject>

#include "Multiplex/Multiplex.h"
#include "Multiplex/MultiplexTable.h"
#include "ObjectTree/ObjectTreeNode.h"
#include "ObjectTree/ObjectTreeList.h"
#include "System/SystemCatalogNode.h"
#include "Device.h"

class Devices : public ObjectTreeNode,
    public ObjectTreeList,
    public SystemCatalogNode,
    public Multiplex
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
     * Edit node, appending a new child: return success.
     */
    virtual bool insertObjectTreeList();
    /*!
     * Edit node, removing the indexed child: return success.
     */
    virtual bool removeObjectTreeList(int idx);
    /*!
     * Update the time series data set.  The caller is responsible for
     * the heap allocation of the arguments, which are copied by this
     * method.
     */
    virtual bool update(const TMTCMessage*);
    /*!
     * Query the time series data set.  The caller is responsible for
     * the returned heap allocation.
     */
    virtual TMTCMessage* query(const TMTCMessage*);
    /*!
     * If the returned value is not a "null qvariant", then the const
     * may be lowered.
     */
    virtual QVariant query(const SystemDeviceIdentifier&, const TMTCName&);
    /*!
     * Plot selector builds the requested set of path structures with
     * visualization into the requested window.
     */
    virtual void select(int count, MultiplexSelect** query, const QRectF& window);

    QList<Device*> listDevices();

    Device* findDevice(const SystemDeviceIdentifier& sid) const;

    MultiplexTable* createMultiplexTable(const SystemDeviceIdentifier& sid);

    MultiplexTable* findMultiplexTable(const SystemDeviceIdentifier& sid) const;

    QList<SystemDeviceIdentifier> findMultiplexTableIdentifiers();

 signals:
    /*!
     * The database emits this signal to transmit to a device
     * connection.
     */
    void sendToDevice(const TMTCMessage*);
    /*!
     * The database emits this signal to transmit to a user interface
     * (e.g. Terminal Output)
     */
    void sendToUser(const TMTCMessage*);

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
    virtual void receivedFromDevice(const TMTCMessage*);
    /*!
     * The database receives this signal from a user interface
     * (e.g. Terminal Input) to query the time series.  
     * 
     * The identifier is assumed to have persistent allocation, and
     * will be employed in the response signal sent to users.
     */
    virtual void receivedFromUser(const TMTCMessage*);

 private:
    Q_DISABLE_COPY(Devices)
};

Q_DECLARE_METATYPE(Devices*)
#endif
