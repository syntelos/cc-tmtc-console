/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QScriptEngine>

#include "Configuration.h"
#include "HCDB.h"


QScriptValue hcdbToScriptValue(QScriptEngine *engine, HCDB* const &in){
    return engine->newQObject(in);
}

void hcdbFromScriptValue(const QScriptValue &object, HCDB* &out){
    out = qobject_cast<HCDB*>(object.toQObject());
}


HCDB::HCDB(CCDB* ccdb, QObject* parent)
    : ObjectTreeNode(parent),
      ccdb(ccdb),
      db(0),
      libraries(0),
      devices(0),
      scripts(0)
{
    initSystemScriptable(this);

    HCDB::init();

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), hcdbToScriptValue, hcdbFromScriptValue);
}
HCDB::~HCDB(){

    if (devices){
        delete devices;
    }

    if (libraries){
        delete libraries;
    }

    if (scripts){
        delete scripts;
    }

    if (db){
        delete db;
    }

    ccdb = 0;
}
bool HCDB::init(){

    if (this->isDown() && ccdb->isReady()){

        const QString* dbDriver = ccdb->getHcdbDriverType();

        const QString* dbName = ccdb->getHcdbDatabaseName();

        db = new QSqlDatabase(QSqlDatabase::addDatabase(*dbDriver,*dbName));

        db->setDatabaseName(*dbName);

        if (db->open()){

            beginInsertNode(0,1);

            libraries = new Libraries(db,this);

            devices = new Devices(db,this);

            scripts = new Scripts(db,this);

            endInsertNode();

            qDebug() << "HCDB init success";

            return true;
        }
        else {
            qDebug() << "HCDB init failure";
            /*
             * TODO (dialog/stderr error: failed to open database)
             */

            delete db;

            db = 0;

            return false;
        }
    }
    else
        return isUp();
}
bool HCDB::isUp(){
    return (libraries);
}
bool HCDB::isDown(){
    return (!libraries);
}
CCDB* HCDB::getCCDB(){

    return ccdb;
}
const QString* HCDB::getHostUuid() const {

    return ccdb->getHostUuid();
}
Libraries* HCDB::getLibraries() const {

    return libraries;
}
Devices* HCDB::getDevices() const {

    return devices;
}
Scripts* HCDB::getScripts() const {

    return scripts;
}
void HCDB::clear(){

    if (devices){

        delete devices;
    }

    if (libraries){

        delete libraries;
    }

    if (scripts){

        delete scripts;
    }
}
bool HCDB::read(){

    return this->init();
}
bool HCDB::write(){

    if (devices && libraries && scripts){

        return (devices->write() && libraries->write() && scripts->write());
    }
    else {

        return false;
    }
}
bool HCDB::done(int error){

    if (error){

        return false;
    }
    else {

        return HCDB::write();
    }
}
