/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QLabel>
#include <QScriptEngine>
#include <QSqlQuery>
#include <QString>
#include <QUuid>

#include "Device.h"
#include "Devices.h"
#include "Configuration.h"
#include "ConfigurationError.h"
#include "XPORT/XportConnection.h"

#define XPORT_SUFFIX 10001


QScriptValue deviceToScriptValue(QScriptEngine *engine, Device* const &in){
    return engine->newQObject(in);
}

void deviceFromScriptValue(const QScriptValue &object, Device* &out){
    out = qobject_cast<Device*>(object.toQObject());
}


Device::Device(QObject* parent)
    : StorageListItem(parent), libraryUuid(vnul), connectionIdentifier(vnul)
{
    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), deviceToScriptValue, deviceFromScriptValue);
}
Device::Device(QSqlQuery& query, int start, QObject* parent)
    : StorageListItem(parent), libraryUuid(vnul), connectionIdentifier(vnul)
{
    this->read(query,start);

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), deviceToScriptValue, deviceFromScriptValue);
}
Device::Device(QSqlQuery& query, QObject* parent)
    : StorageListItem(parent), libraryUuid(vnul), connectionIdentifier(vnul)
{
    this->read(query);

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), deviceToScriptValue, deviceFromScriptValue);
}
Device::~Device(){

    if (libraryUuid){

        delete libraryUuid;
    }

    if (connectionIdentifier){

        delete connectionIdentifier;
    }
}
void Device::read(QSqlQuery& select, int start){

    QString s0 = select.value(start++).toString();

    this->setLibraryUuid(s0);

    QString s1 = select.value(start++).toString();

    this->setConnectionIdentifier(s1);

}
void Device::write(QSqlQuery& insert, int start){


    if (libraryUuid)
        insert.bindValue(start++,*libraryUuid);
    else
        insert.bindValue(start++,QVariant(QVariant::String));


    if (connectionIdentifier)
        insert.bindValue(start++,*connectionIdentifier);
    else
        insert.bindValue(start++,QVariant(QVariant::String));


}
const QString* Device::getHostUuid() const {

    Devices* parent = qobject_cast<Devices*>(this->parent());

    return parent->getHostUuid();
}
const QString* Device::getLibraryUuid() const {

    return this->libraryUuid;
}
void Device::setLibraryUuid(QString* libraryUuid){
    if (this->libraryUuid){
        delete this->libraryUuid;
    }

    if (libraryUuid){

        if (0 < libraryUuid->length()){

            this->libraryUuid = libraryUuid;
        }
        else {
            delete libraryUuid;

            this->libraryUuid = vnul;
        }
    }
    else {

        this->libraryUuid = vnul;
    }
}
void Device::setLibraryUuid(QString& libraryUuid){
    if (this->libraryUuid){
        delete this->libraryUuid;
    }

    if (0 < libraryUuid.length()){

        this->libraryUuid = new QString(libraryUuid);
    }
    else {

        this->libraryUuid = vnul;
    }
}
const SystemDeviceIdentifier* Device::getSystemDeviceIdentifier() const {
    QString* cid = this->connectionIdentifier;
    if (cid){

        quint16 defaultSuffix = XPORT_SUFFIX;

        if (this->libraryUuid){
            // [TODO] library defaultSuffix
        }

        return new SystemDeviceIdentifier(*cid,defaultSuffix);
    }
    else
        return 0;
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
         * [TODO] library
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
const QString* Device::getConnectionIdentifier() const {

    return this->connectionIdentifier;
}
void Device::setConnectionIdentifier(QString* connectionIdentifier){
    if (this->connectionIdentifier){
        delete this->connectionIdentifier;
    }

    if (connectionIdentifier){

        if (0 < connectionIdentifier->length()){

            this->connectionIdentifier = connectionIdentifier;

            this->setObjectName(*connectionIdentifier);
        }
        else {
            delete connectionIdentifier;

            this->connectionIdentifier = vnul;

            QString empty;

            this->setObjectName(empty);
        }
    }
    else {

        this->connectionIdentifier = vnul;

        QString empty;

        this->setObjectName(empty);
    }
    this->nodeDataChanged("connectionIdentifier");
}
void Device::setConnectionIdentifier(QString& connectionIdentifier){
    if (this->connectionIdentifier){
        delete this->connectionIdentifier;
    }

    if (0 < connectionIdentifier.length()){

        this->connectionIdentifier = new QString(connectionIdentifier);

        this->setObjectName(connectionIdentifier);
    }
    else {

        this->connectionIdentifier = vnul;

        QString empty;

        this->setObjectName(empty);
    }
    this->nodeDataChanged("connectionIdentifier");
}
QWidget* Device::createPropertyFormEditor(int index, const QMetaProperty& property){
    /*
     * Accept default
     */
    return vnul;
}
QWidget* Device::createPropertyFormLabel(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("libraryUuid",propName)){

        return new QLabel("Library UUID");
    }
    else if ( 0 == strcmp("connectionIdentifier",propName)){

        return new QLabel("Connect to");
    }
    else {
        /*
         * Accept default
         */
        return vnul;
    }
}
bool Device::setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor){

    return false;
}
bool Device::isInert(){

    return (vnul == connectionIdentifier);
}
