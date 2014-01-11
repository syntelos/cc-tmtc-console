/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QObjectList>
#include <QScriptEngine>
#include <QSqlQuery>
#include <QString>

#include "Libraries.h"
#include "Configuration.h"
#include "ConfigurationError.h"


QScriptValue librariesToScriptValue(QScriptEngine *engine, Libraries* const &in){
    return engine->newQObject(in);
}

void librariesFromScriptValue(const QScriptValue &object, Libraries* &out){
    out = qobject_cast<Libraries*>(object.toQObject());
}


Libraries::Libraries(QSqlDatabase* db, QObject* parent)
    : StorageList(parent), hcdb(db)
{
    if (hcdb->isValid()){

        if (!read()){

            init();
        }
    }
    else {
        throw ConfigurationError::DatabaseDriver("HCDB/Libraries");
    }
    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), librariesToScriptValue, librariesFromScriptValue);
}
Libraries::~Libraries(){

    this->clear();

    hcdb = vnul;

}
void Libraries::init(){

    this->clear();

    /*
     * DROP
     */
    QString dsql("DROP TABLE ");
    dsql += HcdbLibraryTableName;
    dsql += ";";

    QSqlQuery drop(dsql,*hcdb);

    bool dropExec = drop.exec();
    /*
     * CREATE
     */
    QString csql("CREATE TABLE ");
    csql += HcdbLibraryTableName;
    /*
     * There are many records per host in this table, but only one
     * record per the combination of HOST_UUID and LIBRARY_UUID.
     * 
     * The primary key constraint requires both fields be non-null.
     */
    csql += " ( HOST_UUID CHARACTER(38), LIBRARY_UUID CHARACTER(38), FILE_IDENTIFIER VARCHAR, LANGUAGE_CLASSNAME VARCHAR, CONNECTION_CLASSNAME VARCHAR, PRIMARY KEY ( HOST_UUID, LIBRARY_UUID ) );";

    QSqlQuery create(csql,*hcdb);

    bool createExec = create.exec();/* This returns false even when it's functionally
                                     * operating.
                                     */

    if (createExec)
        qDebug() << "Libraries init success";
    else
        qDebug() << "Libraries init failure";
}
void Libraries::clear(){

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
bool Libraries::read(){

    this->clear();

    const QString* hostUuid = this->getHostUuid();
    /*
     * Many library records per host
     */
    QString sql = "SELECT LIBRARY_UUID, FILE_IDENTIFIER, LANGUAGE_CLASSNAME, CONNECTION_CLASSNAME FROM ";
    sql += HcdbLibraryTableName;
    sql += " WHERE HOST_UUID = '";
    sql += *hostUuid;
    sql += "';";

    QSqlQuery select(sql,*hcdb);

    if (select.exec()){

        const int count = select.size();

        qDebug() << "Libraries read [exec true] [count " << count << "]";

        beginInsertNode(0,(count-1));

        while (select.next()){

            new Library(select,this);
        }

        endInsertNode();

        qDebug() << "Libraries read success";

        emit readSuccess();

        return true;
    }
    else {
        qDebug() << "Libraries read failure";

        emit readFailure();

        return false;
    }
}
bool Libraries::write(){

    const QString* hostUuid = this->getHostUuid();

    QSqlQuery op(*hcdb);
    /*
     * Delete everything
     */
    QString dsql("DELETE FROM ");
    dsql += HcdbLibraryTableName;
    dsql += " WHERE HOST_UUID = '";
    dsql += *hostUuid;
    dsql += "';";

    if (op.prepare(dsql) && op.exec()){
        /*
         * Insert what we have
         */
        QString isql("INSERT INTO ");
        isql += HcdbLibraryTableName;
        isql += " ( HOST_UUID, LIBRARY_UUID, FILE_IDENTIFIER, LANGUAGE_CLASSNAME, CONNECTION_CLASSNAME ) VALUES ( '";
        isql += *hostUuid;
        isql += "',  ?, ?, ?, ? );";


        if (op.prepare(isql)){

            const QObjectList& children = this->children();

            const int sz = children.size();

            if (0 < sz){

                //beginStoreNode

                int cc;
                for (cc = 0; cc < sz; cc++){

                    Library* child = qobject_cast<Library*>(children.at(cc));

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
bool Libraries::done(int write){

    if (0 == write){

        return Libraries::write();
    }
    else {

        return false;
    }
}
const QString* Libraries::getHostUuid() const {

    HCDB* parent = qobject_cast<HCDB*>(this->parent());

    return parent->getHostUuid();
}
bool Libraries::insertObjectTreeList(){
    const QObjectList& children = this->children();
    const int index = children.size();

    if (0 == index){

        beginInsertNode(index,index);

        (new Library(this));

        endInsertNode();

        return true;
    }
    else {
        Library* last = static_cast<Library*>(children.last());

        if (last->isInert()){

            return false;
        }
        else {
            beginInsertNode(index,index);

            (new Library(this));

            endInsertNode();

            return true;
        }
    }
}
bool Libraries::removeObjectTreeList(int idx){
    if (-1 < idx){
        QObjectList& children = const_cast<QObjectList&>(this->children());
        if (idx < children.size()){

            beginRemoveNode(idx,idx);

            delete children.takeAt(idx);

            endRemoveNode();

            return true;
        }
    }
    return false;
}
