/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QLabel>
#include <QList>
#include <QString>

#include "Device.h"
#include "Devices.h"
#include "System/SystemDeviceConnectionConstructorDiscovery.h"
#include "System/SystemConnectionTCP.h"
#include "System/SystemMultiplexTableMem.h"
#include "System/SystemMultiplexTableConstructorDiscovery.h"

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
    : SystemDevice(parent), library(0), identifier(sid)
{
    setObjectName(sid.toString("device"));
}
Device::~Device(){

    if (library){

        delete library;
    }
}
QString* Device::getLibrary() const {

    return this->library;
}
void Device::setLibrary(QString* library){

    if (this->library){

        if (library != this->library)
            delete this->library;
        else
            return;
    }

    if (library){

        if (0 < library->length()){

            this->library = library;
        }
        else {
            delete library;

            this->library = 0;
        }
    }
    else {

        this->library = 0;
    }
}
void Device::setLibrary(QString& library){

    if (this->library){

        if (&library != this->library)
            delete this->library;
        else
            return;
    }

    if (0 < library.length()){

        this->library = new QString(library);
    }
    else {

        this->library = 0;
    }
}
void Device::clear(){

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
SystemMultiplex* Device::getSystemMultiplex() const {

    return dynamic_cast<SystemMultiplex*>(parent());
}
SystemMultiplexTable* Device::findSystemMultiplexTable() const {

    const QObjectList& children = this->children();
    foreach(QObject* child, children){

        SystemMultiplexTable* table = dynamic_cast<SystemMultiplexTable*>(child);
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
void Device::start(){

    SystemCatalogNode::start(this); // prop
}
void Device::stop(){

    SystemCatalogNode::stop(this); // prop
}
void Device::read(const SystemCatalogInput& properties, const QDomElement& node){

    this->clear();

    QString nodeName = node.localName();
    if (nodeName == "device"){

        QString nodeId = node.attribute("id");

        QDomNodeList children = node.childNodes();
        const uint count = children.length();
        bool inputReceiver = true;

        SystemMultiplex* multiplex = getSystemMultiplex();

        uint cc;
        for (cc = 0; cc < count; cc++){
            QDomNode child = children.item(cc);
            if (child.isElement()){

                QString name = child.localName();

                QDomElement cel = child.toElement();

                if (name == "connection"){

                    SystemDeviceConnection* connection = 0;
                    /*
                     * Connection binding: static and dynamic
                     */
                    QString connectionClass = node.attribute("class");
                    if (!connectionClass.isEmpty()){
                        QString cclc = connectionClass.toLower();
                        if (cclc == "tcp"){

                            connection = new SystemConnectionTCP(identifier,this);
                        }
                        else {

                            SystemDeviceConnectionConstructorDiscovery ctor(connectionClass);

                            connection = ctor.construct(identifier,this);
                        }
                    }
                    else {
                        connection = new SystemConnectionTCP(identifier,this);
                    }
                    /*
                     * [TODO] connection->read (prevent duplication)
                     */
                    if (connection){

                        QObject::connect(connection,SIGNAL(received(const SystemMessage*)),multiplex,SLOT(receivedFromDevice(const SystemMessage*)));

                        QObject::connect(multiplex,SIGNAL(sendToDevice(const SystemMessage*)),connection,SLOT(send(const SystemMessage*)));
                    }
                    else {
                        qDebug() << "Device.read: skipping connection for" << nodeId;
                    }
                }
                else if (name == "table"){

                    SystemMultiplexTable* table = 0;
                    /*
                     * Table binding: static and dynamic
                     */
                    QString tableClass = node.attribute("class");
                    if (tableClass.isEmpty()){

                        table = new SystemMultiplexTableMem(identifier,this);
                    }
                    else {
                        QString tclc = tableClass.toLower();
                        if (tclc == "mem"){
                            table = new SystemMultiplexTableMem(identifier,this);
                        }
                        else {
                            SystemMultiplexTableConstructorDiscovery ctor(tableClass);

                            table = ctor.construct(identifier,this);
                        }
                    }
                    /*
                     */
                    if (table){

                        table->read(properties,cel);
                    }
                    else {
                        qDebug() << "Device.read: skipping table for" << nodeId;
                    }
                }
                else if (name == "connect"){

                    if (readConnect(this,properties,node,cel)){

                        inputReceiver = false;
                    }
                }
                else {
                    qDebug() << "Device.read: skipping unrecognized element" << name ;
                }
            }
        }

        if (inputReceiver){

            if (!nodeId.isEmpty()){
                properties.receiver(nodeId,this);
            }
        }
    }
    else {
        qDebug() << "Device.read: Unrecognized node element" << nodeName;
    }
}
void Device::write(SystemCatalogOutput& properties, QDomElement& node){
}
