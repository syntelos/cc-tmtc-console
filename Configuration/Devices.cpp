/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QString>

#include "Devices.h"
#include "Configuration.h"
#include "System/SystemDeviceConstructorDiscovery.h"


QScriptValue devicesToScriptValue(QScriptEngine *engine, Devices* const &in){
    return engine->newQObject(in);
}

void devicesFromScriptValue(const QScriptValue &object, Devices* &out){
    out = qobject_cast<Devices*>(object.toQObject());
}

void Devices::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, devicesToScriptValue, devicesFromScriptValue);

    Device::InitScriptMetaType(engine);
}

Devices::Devices(QObject* parent)
    : SystemMultiplex(parent)
{
}
Devices::~Devices(){

    this->clear();
}
void Devices::clear(){

    QObjectList& children = const_cast<QObjectList&>(this->children());

    const int sz = children.size();

    if (0 < sz){

        int cc;
        for (cc = 0; cc < sz; cc++){

            QObject* child = children.takeAt(cc);

            if (child){

                child->deleteLater(); // (could be in a view)
            }
        }
    }
}
void Devices::start(){
    SystemCatalogNode::start(this);
}
void Devices::stop(){
    SystemCatalogNode::stop(this);
}
void Devices::read(const SystemCatalogInput& properties, const QDomElement& node){

    this->clear();

    if (node.localName() == "devices"){
        QDomNodeList children = node.childNodes();
        const uint count = children.length();
        bool inputReceiver = true;
        uint cc;
        for (cc = 0; cc < count; cc++){
            QDomNode child = children.item(cc);
            if (child.isElement()){

                QString name = child.localName();

                QDomElement cel = child.toElement();

                if (name == "device"){
                    /*
                     * http://www.w3.org/TR/xml-id/ (SystemDeviceIdentifier == "<ip_addr>:<ip_port>")
                     */
                    QString deviceId = node.attribute("id");
                    if (!deviceId.isEmpty()){
                        const SystemDeviceIdentifier& sid = SystemDeviceIdentifier::intern(deviceId);

                        Device* device = 0;
                        /*
                         * Device binding: static and dynamic
                         */
                        QString deviceClass = node.attribute("class");
                        if (!deviceClass.isEmpty() && deviceClass != "Device"){

                            SystemDeviceConstructorDiscovery ctor(deviceClass);
                            SystemDevice* sys = ctor.construct(sid,this);
                            if (sys){
                                device = static_cast<Device*>(sys);
                            }
                            else {
                                qDebug() << "Devices.read: unable to construct an instance of class" << deviceClass << "for" << deviceId;
                            }
                        }
                        else {
                            device = new Device(sid,this);
                        }
                        /*
                         * Device configuration
                         */
                        if (device){
                            device->read(properties,cel);
                        }
                        else {
                            qDebug() << "Devices.read: skipping device" << deviceId;
                        }
                    }
                    else {
                        qDebug() << "Devices.read: skipping device missing attribute 'id'.";
                    }
                }
                else if (name == "connect"){

                    if (readConnect(this,properties,node,cel)){

                        inputReceiver = false;
                    }
                }
                else {
                    qDebug() << "Devices.read: skipping unrecognized element" << name ;
                }
            }
        }

        if (inputReceiver){
            QString receiverId = node.attribute("id");
            if (!receiverId.isEmpty()){
                properties.receiver(receiverId,this);
            }
        }
    }
    else {
        qDebug() << "Devices.read: Unrecognized node element" << node.localName();
    }

}
void Devices::write(SystemCatalogOutput& properties, QDomElement& node){


    const QObjectList& children = this->children();

    const int sz = children.size();

    if (0 < sz){

        //beginStoreNode

        int cc;
        for (cc = 0; cc < sz; cc++){

            Device* child = qobject_cast<Device*>(children.at(cc));

            if (child){

                //child->write();

            }
        }

        //endStoreNode
    }
}
Device* Devices::findDevice(const SystemDeviceIdentifier& sid) const {

    const Device* device = findChild<Device*>(sid.getPrefix());

    return const_cast<Device*>(device);
}
SystemMultiplexTable* Devices::findSystemMultiplexTable(const SystemDeviceIdentifier& sid) const {

    const Device* device = findDevice(sid);

    if (device)
        return device->findSystemMultiplexTable();
    else
        return 0;
}
QList<Device*> Devices::listDevices(){
    QList<Device*> re;
    const QObjectList& children = this->children();
    foreach(QObject* child, children){
        Device* device = dynamic_cast<Device*>(child);
        if (device){

            re += device;
        }
    }
    return re;
}
QList<SystemDeviceIdentifier> Devices::findSystemMultiplexTableIdentifiers(){
    QList<SystemDeviceIdentifier> re;
    const QObjectList& children = this->children();
    foreach(QObject* child, children){
        Device* device = dynamic_cast<Device*>(child);
        if (device){
            SystemMultiplexTable* table = device->findSystemMultiplexTable();

            if (table){

                re += table->getSystemDeviceIdentifier();
            }
        }
    }
    return re;
}
bool Devices::update(const SystemMessage* m){
    if (m){

        const SystemDeviceIdentifier& sid = m->getIdentifier();

        if (sid.isValid() || sid.isSpecial()){

            SystemMultiplexTable* history = findSystemMultiplexTable(sid);
            if (history){
                /*
                 * An invalid (special) ID will deposit values into the
                 * referenced data set.  The values in the "special" data
                 * set have "desktop" semantics: they cannot be proxied to
                 * an Instrument Device (Connection).
                 */
                history->update(*m);

                return true;
            }
        }
    }
    return false;
}
SystemMessage* Devices::query(const SystemMessage* m){
    if (m){

        const SystemDeviceIdentifier& sid = m->getIdentifier();

        if (sid.isValid()){

            SystemMultiplexTable* history = findSystemMultiplexTable(sid);

            if (history)
                return history->query(*m);
            else
                return 0;
        }
        else if (sid.isSpecial()){

            SystemMessage* re = new SystemMessage(sid);

            if (m->isSpecial()){
                /*
                 * Use desktop semantics
                 */
                SystemMultiplexTable* special = findSystemMultiplexTable(sid);
                if (special){

                    QList<SystemNameValue*>::const_iterator p, z;
                    for (p = m->constBegin(), z = m->constEnd(); p != z; ++p){
                        const SystemNameValue* nvp = *p;

                        if (nvp->hasValue()){
                            /*
                             * Update
                             */
                            special->update(*nvp);
                        }
                        /*
                         * Query
                         */
                        const SystemName& n = nvp->getName();
                        QVariant v = special->query(n);

                        if (v.isValid()){
                            /*
                             * Query response
                             */
                            re->append(new SystemNameValue(n,v));
                        }
                    }
                }
            }
            else {
                /*
                 * Use broadcast semantics
                 */
                QList<Device*> devices = listDevices();
                QList<Device*>::const_iterator dp, dz;
                for (dp = devices.constBegin(), dz = devices.constEnd(); dp != dz; ++dp){

                    SystemMultiplexTable* history = (*dp)->findSystemMultiplexTable();

                    if (history){

                        QList<SystemNameValue*>::const_iterator p, z;
                        for (p = m->constBegin(), z = m->constEnd(); p != z; ++p){
                            const SystemNameValue* nvp = *p;

                            if (nvp->hasNotValue()){

                                const SystemName& n = nvp->getName();
                                QVariant v = history->query(n);

                                if (v.isValid()){

                                    re->append(new SystemNameValue(n,v));
                                }
                            }
                        }
                    }
                }
            }

            return re;
        }
    }
    return 0;
}
QVariant Devices::query(const SystemDeviceIdentifier& sid, const SystemName& n){

    if (sid.isValid()){

        SystemMultiplexTable* history = findSystemMultiplexTable(sid);

        if (history){
            return history->query(n);
        }
    }
    QVariant nil;
    return nil;
}
void Devices::select(int count, SystemMultiplexSelect** query, const QRectF& window){
    if (0 < count){

        int cc;
        for (cc = 0; cc < count; cc++){

            SystemMultiplexSelect& select = *query[cc];

            const SystemDeviceIdentifier& sid = select.id;

            if (sid.isValid()){

                SystemMultiplexTable* history = findSystemMultiplexTable(sid);

                if (history){
                    history->select(select);
                }
            }
        }
        /*
         * Determine boundaries of results
         */
        qreal min_x = SystemMultiplexSelect::InitMin;
        qreal min_y = SystemMultiplexSelect::InitMin;
        qreal max_x = SystemMultiplexSelect::InitMax;
        qreal max_y = SystemMultiplexSelect::InitMax;

        for (cc = 0; cc < count; cc++){

            SystemMultiplexSelect& select = *query[cc];

            if (select.local_min_x < min_x)
                min_x = select.local_min_x;
            else if (select.local_max_x > max_x)
                max_x = select.local_max_x;

            if (select.local_min_y < min_y)
                min_y = select.local_min_y;
            else if (select.local_max_y > max_y)
                max_y = select.local_max_y;
        }


        //const qreal wx = window.x();
        //const qreal wy = window.y();
        const qreal ww = window.width();
        const qreal wh = window.height();
        /*
         * Autoscaling all results into one scale
         */
        const qreal sx = (ww/(max_x-min_x));
        const qreal sy = (wh/(max_y-min_y));

        for (cc = 0; cc < count; cc++){

            SystemMultiplexSelect& select = *query[cc];

            select.global_min_x = min_x;
            select.global_max_x = max_x;
            select.global_min_y = min_y;
            select.global_max_y = max_y;

            select.visualization(window,sx,sy);
        }
    }
}
void Devices::receivedFromDevice(const SystemMessage* m){

    if (m && this->update(m)){

        emit sendToUser(m);
    }
}
void Devices::receivedFromUser(const SystemMessage* m){

    SystemMessage* re = this->query(m);
    if (re){

        emit sendToUser(re);

        re->deleteLater();
    }
}
