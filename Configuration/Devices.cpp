/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QString>

#include "Devices.h"
#include "Configuration.h"


QScriptValue devicesToScriptValue(QScriptEngine *engine, Devices* const &in){
    return engine->newQObject(in);
}

void devicesFromScriptValue(const QScriptValue &object, Devices* &out){
    out = qobject_cast<Devices*>(object.toQObject());
}

void Devices::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, devicesToScriptValue, devicesFromScriptValue);
}

Devices::Devices(QObject* parent)
    : ObjectTreeNode(parent)
{
}
Devices::~Devices(){

    this->clear();
}
void Devices::clear(){

    QObjectList& children = const_cast<QObjectList&>(this->children());

    const int sz = children.size();

    if (0 < sz){

        beginRemoveNode(0,(sz-1));

        int cc;
        for (cc = 0; cc < sz; cc++){

            QObject* child = children.takeAt(cc);

            if (child){

                child->deleteLater(); // (could be in a view)
            }
        }

        endRemoveNode();
    }
}
void Devices::start(){
}
void Devices::stop(){
}
void Devices::read(const SystemCatalogInput& properties, const QDomElement& parent){

    this->clear();


    // beginInsertNode(0,(count-1));

    // while (select.next()){

    //     new Device(this);
    // }

    // Device* device = static_cast<Device*>(children.at(cc));

    // SystemDeviceConnection* connection = const_cast<SystemDeviceConnection*>(device->createSystemDeviceConnection());

    // if (connection &&
    //     QObject::connect(connection,SIGNAL(received(const TMTCMessage*)),multiplex,SLOT(receivedFromDevice(const TMTCMessage*))) &&
    //     QObject::connect(multiplex,SIGNAL(sendToDevice(const TMTCMessage*)),connection,SLOT(send(const TMTCMessage*)))
    //     )


    // endInsertNode();

}
void Devices::write(SystemCatalogOutput& properties, QDomElement& parent){


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
bool Devices::insertObjectTreeList(){
    const QObjectList& children = this->children();
    const int index = children.size();
    if (0 == index){

        beginInsertNode(index,index);

        //(new Device(this));

        endInsertNode();

        return true;
    }
    else {

        beginInsertNode(index,index);

        //(new Device(this));

        endInsertNode();

        return true;
    }
}
bool Devices::removeObjectTreeList(int idx){
    if (-1 < idx){
        QObjectList& children = const_cast<QObjectList&>(this->children());
        if (idx < children.size()){

            beginInsertNode(idx,idx);

            delete children.takeAt(idx);

            endInsertNode();

            return true;
        }
    }
    return false;
}

Device* Devices::findDevice(const SystemDeviceIdentifier& sid) const {

    const Device* device = findChild<Device*>(sid.getPrefix());

    return const_cast<Device*>(device);
}
MultiplexTable* Devices::findMultiplexTable(const SystemDeviceIdentifier& sid) const {

    const Device* device = findDevice(sid);

    if (device)
        return device->findMultiplexTable();
    else
        return 0;
}
MultiplexTable* Devices::createMultiplexTable(const SystemDeviceIdentifier& sid){

    Device* device = findDevice(sid);

    if (device)
        return device->createMultiplexTable();
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
QList<SystemDeviceIdentifier> Devices::findMultiplexTableIdentifiers(){
    QList<SystemDeviceIdentifier> re;
    const QObjectList& children = this->children();
    foreach(QObject* child, children){
        Device* device = dynamic_cast<Device*>(child);
        if (device){
            MultiplexTable* table = device->findMultiplexTable();

            if (table){

                re += table->getSystemDeviceIdentifier();
            }
        }
    }
    return re;
}
bool Devices::update(const TMTCMessage* m){
    if (m){

        const SystemDeviceIdentifier& sid = m->getIdentifier();

        if (sid.isValid() || sid.isSpecial()){

            MultiplexTable* history = createMultiplexTable(sid);
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
    return false;
}
TMTCMessage* Devices::query(const TMTCMessage* m){
    if (m){

        const SystemDeviceIdentifier& sid = m->getIdentifier();

        if (sid.isValid()){

            MultiplexTable* history = findMultiplexTable(sid);

            if (history)
                return history->query(*m);
            else
                return 0;
        }
        else if (sid.isSpecial()){

            TMTCMessage* re = new TMTCMessage(sid);

            if (m->isSpecial()){
                /*
                 * Use desktop semantics
                 */
                MultiplexTable* special = createMultiplexTable(sid);

                QList<TMTCNameValue*>::const_iterator p, z;
                for (p = m->constBegin(), z = m->constEnd(); p != z; ++p){
                    const TMTCNameValue* nvp = *p;

                    if (nvp->hasValue()){
                        /*
                         * Update
                         */
                        special->update(*nvp);
                    }
                    /*
                     * Query
                     */
                    const TMTCName& n = nvp->getName();
                    QVariant v = special->query(n);

                    if (v.isValid()){
                        /*
                         * Query response
                         */
                        re->append(new TMTCNameValue(n,v));
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

                    MultiplexTable* history = (*dp)->findMultiplexTable();

                    if (history){

                        QList<TMTCNameValue*>::const_iterator p, z;
                        for (p = m->constBegin(), z = m->constEnd(); p != z; ++p){
                            const TMTCNameValue* nvp = *p;

                            if (nvp->hasNotValue()){

                                const TMTCName& n = nvp->getName();
                                QVariant v = history->query(n);

                                if (v.isValid()){

                                    re->append(new TMTCNameValue(n,v));
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
QVariant Devices::query(const SystemDeviceIdentifier& sid, const TMTCName& n){

    if (sid.isValid()){

        MultiplexTable* history = findMultiplexTable(sid);

        if (history){
            return history->query(n);
        }
    }
    QVariant nil;
    return nil;
}
void Devices::select(int count, MultiplexSelect** query, const QRectF& window){
    if (0 < count){

        int cc;
        for (cc = 0; cc < count; cc++){

            MultiplexSelect& select = *query[cc];

            const SystemDeviceIdentifier& sid = select.id;

            if (sid.isValid()){

                MultiplexTable* history = findMultiplexTable(sid);

                if (history){
                    history->select(select);
                }
            }
        }
        /*
         * Determine boundaries of results
         */
        qreal min_x = MultiplexSelect::InitMin;
        qreal min_y = MultiplexSelect::InitMin;
        qreal max_x = MultiplexSelect::InitMax;
        qreal max_y = MultiplexSelect::InitMax;

        for (cc = 0; cc < count; cc++){

            MultiplexSelect& select = *query[cc];

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

            MultiplexSelect& select = *query[cc];

            select.global_min_x = min_x;
            select.global_max_x = max_x;
            select.global_min_y = min_y;
            select.global_max_y = max_y;

            select.visualization(window,sx,sy);
        }
    }
}
void Devices::receivedFromDevice(const TMTCMessage* m){

    if (m && this->update(m)){

        emit sendToUser(m);
    }
}
void Devices::receivedFromUser(const TMTCMessage* m){

    TMTCMessage* re = this->query(m);
    if (re){

        emit sendToUser(re);

        re->deleteLater();
    }
}
