/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QObjectList>
#include <QScriptEngine>
#include <QSqlQuery>
#include <QString>

#include "Devices.h"
#include "Configuration.h"
#include "ConfigurationError.h"


QScriptValue devicesToScriptValue(QScriptEngine *engine, Devices* const &in){
    return engine->newQObject(in);
}

void devicesFromScriptValue(const QScriptValue &object, Devices* &out){
    out = qobject_cast<Devices*>(object.toQObject());
}


Devices::Devices(QSqlDatabase* db, QObject* parent)
    : StorageList(parent), hcdb(db)
{
    if (hcdb->isValid()){

        if (!read()){

            init();
        }
    }
    else {
        throw ConfigurationError::DatabaseDriver("HCDB/Devices");
    }
    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), devicesToScriptValue, devicesFromScriptValue);

}
Devices::~Devices(){

    this->clear();

    hcdb = vnul;

}
void Devices::init(){

    this->clear();

    /*
     * DROP
     */
    QString dsql("DROP TABLE ");
    dsql += HcdbDeviceTableName;
    dsql += ";";

    QSqlQuery drop(dsql,*hcdb);

    bool dropExec = drop.exec();
    /*
     * CREATE
     */
    QString csql("CREATE TABLE ");
    csql += HcdbDeviceTableName;
    /*
     * There's many device records per host in this table, so the
     * HOST_UUID is not UNIQUE.
     * 
     * The LIBRARY_UUID may be null, indicating the default builtin
     * connectivity library.
     * 
     * The CONNECTION_IDENTIFIER is an IP Address or Port Name string
     * for the library's connection class constructor.
     */
    csql += " ( HOST_UUID CHARACTER(38), LIBRARY_UUID CHARACTER(38), CONNECTION_IDENTIFIER VARCHAR, PRIMARY KEY ( HOST_UUID, CONNECTION_IDENTIFIER ) );";

    QSqlQuery create(csql,*hcdb);

    bool createExec = create.exec();/* This returns false even when it's functionally
                                     * operating.
                                     */
    if (createExec)
        qDebug() << "Libraries init success";
    else
        qDebug() << "Libraries init failure";
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
bool Devices::read(){

    this->clear();

    const QString* hostUuid = this->getHostUuid();
    /*
     * Many device records per host
     */
    QString sql = "SELECT LIBRARY_UUID, CONNECTION_IDENTIFIER FROM ";
    sql += HcdbDeviceTableName;
    sql += " WHERE HOST_UUID = '";
    sql += *hostUuid;
    sql += "';";

    QSqlQuery select(sql,*hcdb);

    if (select.exec()){

        const int count = select.size();

        qDebug() << "Devices read [exec true] [count " << count << "]";

        beginInsertNode(0,(count-1));

        while (select.next()){

            new Device(select,this);
        }

        endInsertNode();

        qDebug() << "Devices read success";

        emit readSuccess();

        return true;
    }
    else {
        qDebug() << "Devices read failure";

        emit readFailure();

        return false;
    }
}
bool Devices::write(){

    const QString* hostUuid = this->getHostUuid();

    QSqlQuery op(*hcdb);
    /*
     * Delete everything
     */
    QString dsql("DELETE FROM ");
    dsql += HcdbDeviceTableName;
    dsql += " WHERE HOST_UUID = '";
    dsql += *hostUuid;
    dsql += "';";

    if (op.prepare(dsql) && op.exec()){
        /*
         * Insert what we have
         */
        QString isql("INSERT INTO ");
        isql += HcdbDeviceTableName;
        isql += " ( HOST_UUID, LIBRARY_UUID, CONNECTION_IDENTIFIER ) VALUES ( '";
        isql += *hostUuid;
        isql += "',  ?, ? );";


        if (op.prepare(isql)){

            const QObjectList& children = this->children();

            const int sz = children.size();

            if (0 < sz){

                //beginStoreNode

                int cc;
                for (cc = 0; cc < sz; cc++){

                    Device* child = qobject_cast<Device*>(children.at(cc));

                    if (child){

                        child->write(op);

                        bool insertExec = op.exec();
                    }
                }

                //endStoreNode
            }

            emit writeSuccess();

            return true;
        }
    }

    emit readFailure();

    return false;
}
bool Devices::done(int write){

    if (0 == write){

        return Devices::write();
    }
    else {

        return false;
    }
}
const QString* Devices::getHostUuid() const {

    HCDB* parent = qobject_cast<HCDB*>(this->parent());

    return parent->getHostUuid();
}
bool Devices::insertObjectTreeList(){
    const QObjectList& children = this->children();
    const int index = children.size();
    if (0 == index){

        beginInsertNode(index,index);

        (new Device(this));

        endInsertNode();

        return true;
    }
    else {
        Device* last = static_cast<Device*>(children.last());

        if (last->isInert()){

            return false;
        }
        else {
            beginInsertNode(index,index);

            (new Device(this));

            endInsertNode();

            return true;
        }
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
