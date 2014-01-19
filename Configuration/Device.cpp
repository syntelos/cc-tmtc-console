/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QLabel>
#include <QList>
#include <QString>
#include <QUuid>

#include "Device.h"
#include "Devices.h"
#include "XPORT/XportConnection.h"

#define XPORT_SUFFIX 10001


QScriptValue deviceToScriptValue(QScriptEngine *engine, Device* const &in){
    return engine->newQObject(in);
}

void deviceFromScriptValue(const QScriptValue &object, Device* &out){
    out = qobject_cast<Device*>(object.toQObject());
}

void Device::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, deviceToScriptValue, deviceFromScriptValue);
}

Device::Device(const SystemDeviceIdentifier& sid, QObject* parent)
    : ObjectTreeNode(parent), libraryUuid(0), identifier(sid)
{
    setObjectName(sid.toString("device"));
}
Device::~Device(){

    if (libraryUuid){

        delete libraryUuid;
    }

    delete &identifier;
}
QString* Device::getLibraryUuid() const {

    return this->libraryUuid;
}
void Device::setLibraryUuid(QString* libraryUuid){

    if (this->libraryUuid){

        if (libraryUuid != this->libraryUuid)
            delete this->libraryUuid;
        else
            return;
    }

    if (libraryUuid){

        if (0 < libraryUuid->length()){

            this->libraryUuid = libraryUuid;
        }
        else {
            delete libraryUuid;

            this->libraryUuid = 0;
        }
    }
    else {

        this->libraryUuid = 0;
    }
}
void Device::setLibraryUuid(QString& libraryUuid){

    if (this->libraryUuid){

        if (&libraryUuid != this->libraryUuid)
            delete this->libraryUuid;
        else
            return;
    }

    if (0 < libraryUuid.length()){

        this->libraryUuid = new QString(libraryUuid);
    }
    else {

        this->libraryUuid = 0;
    }
}
MultiplexTable* Device::createMultiplexTable(){

    MultiplexTable* table = findMultiplexTable();

    if (0 != table)
        return table;
    else {

        table = new MultiplexTable(identifier,this);

        if (table->open()){
            qDebug().nospace() << "Device.createMultiplexTable [open] " << identifier.toString() << " OK";
        }
        else {
            qDebug().nospace() << "Device.createMultiplexTable [open] " << identifier.toString() << " ERROR";
        }
        return table;
    }
}
MultiplexTable* Device::findMultiplexTable() const {

    const QObjectList& children = this->children();
    foreach(QObject* child, children){

        MultiplexTable* table = dynamic_cast<MultiplexTable*>(child);
        if (table){

            return table;
        }
    }
    return 0;
}
const SystemDeviceIdentifier& Device::getSystemDeviceIdentifier() const {

    return identifier;
}
const SystemDeviceConnection* Device::getSystemDeviceConnection() const {

    const QList<QObject*> children = this->children();
    const int count = children.size();
    int cc;
    for (cc = 0; cc < count; cc++){
        SystemDeviceConnection* test = dynamic_cast<SystemDeviceConnection*>(children.at(cc));
        if (test){

            return test;
        }
    }
    return 0;
}
const SystemDeviceConnection* Device::createSystemDeviceConnection(){
    const SystemDeviceConnection* test = this->getSystemDeviceConnection();
    if (test)
        return test;
    else {
        /*
         * [TODO] re/arch connection {tcp,fsdev,http,xmpp}
         */
        return new XportConnection(this);
    }
}
void Device::shutdownSystemDeviceConnection(){
    SystemDeviceConnection* test = const_cast<SystemDeviceConnection*>(this->getSystemDeviceConnection());
    if (test){
        test->shutdown();
    }
}
QWidget* Device::createPropertyFormEditor(int index, const QMetaProperty& property){
    /*
     * Accept default
     */
    return 0;
}
QWidget* Device::createPropertyFormLabel(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("libraryUuid",propName)){

        return new QLabel("Library UUID");
    }
    else if ( 0 == strcmp("systemDeviceIdentifier",propName)){

        return new QLabel("Connected to");
    }
    else {
        /*
         * Accept default
         */
        return 0;
    }
}
bool Device::setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor){

    return false;
}
