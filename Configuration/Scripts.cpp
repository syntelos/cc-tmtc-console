/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QLabel>
#include <QObjectList>
#include <QScriptEngine>
#include <QSqlQuery>
#include <QString>

#include "Scripts.h"
#include "Configuration.h"
#include "ConfigurationError.h"



QScriptValue scriptsToScriptValue(QScriptEngine *engine, Scripts* const &in){
    return engine->newQObject(in);
}

void scriptsFromScriptValue(const QScriptValue &object, Scripts* &out){
    out = qobject_cast<Scripts*>(object.toQObject());
}


Scripts::Scripts(QSqlDatabase* db, QObject* parent)
    : StorageList(parent), hcdb(db)
{
    SystemScriptSymbol::Init();

    if (hcdb->isValid()){

        if (!read()){

            init();
        }
    }
    else {
        throw ConfigurationError::DatabaseDriver("HCDB/Scripts");
    }
    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), scriptsToScriptValue, scriptsFromScriptValue);
}
Scripts::~Scripts(){

    this->clear();

    hcdb = 0;

}
void Scripts::init(){

    this->clear();
    /*
     * DROP
     */
    QString dsql("DROP TABLE ");
    dsql += HcdbScriptTableName;
    dsql += ";";

    QSqlQuery drop(dsql,*hcdb);

    bool dropExec = drop.exec();
    /*
     * CREATE
     */
    QString csql("CREATE TABLE ");
    csql += HcdbScriptTableName;
    /*
     * There's many script records per host in this table, so the
     * HOST_UUID is not UNIQUE.
     */
    csql += " ( HOST_UUID CHARACTER(38), SOURCE VARCHAR(127), TARGET VARCHAR(127), FILE VARCHAR(255), CONTENT VARCHAR, PRIMARY KEY ( HOST_UUID, SOURCE ) );";

    QSqlQuery create(csql,*hcdb);

    bool createExec = create.exec();/* This returns false even when it's functionally
                                     * operating.
                                     */
}
void Scripts::clear(){

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
bool Scripts::read(){

    this->clear();

    const QString* hostUuid = this->getHostUuid();
    /*
     * Many script records per host
     */
    QString sql = "SELECT SOURCE, TARGET, FILE, CONTENT FROM ";
    sql += HcdbScriptTableName;
    sql += " WHERE HOST_UUID = '";
    sql += *hostUuid;
    sql += "';";

    QSqlQuery select(sql,*hcdb);

    if (select.exec()){

        const int count = select.size();

        qDebug() << "Scripts read [exec true] [count " << count << "]";

        beginInsertNode(0,(count-1));

        while (select.next()){

            new Script(select,this);
        }

        endInsertNode();

        qDebug() << "Scripts read success";

        emit readSuccess();

        return true;
    }
    else {
        qDebug() << "Scripts read failure";

        emit readFailure();

        return false;
    }
}
bool Scripts::write(){

    const QString* hostUuid = this->getHostUuid();

    QSqlQuery op(*hcdb);
    /*
     * Delete everything
     */
    QString dsql("DELETE FROM ");
    dsql += HcdbScriptTableName;
    dsql += " WHERE HOST_UUID = '";
    dsql += *hostUuid;
    dsql += "';";

    if (op.prepare(dsql) && op.exec()){
        /*
         * Insert what we have
         */
        QString isql("INSERT INTO ");
        isql += HcdbScriptTableName;
        isql += " ( HOST_UUID, SOURCE, TARGET, FILE, CONTENT ) VALUES ( '";
        isql += *hostUuid;
        isql += "',  ?, ?, ?, ? );";


        if (op.prepare(isql)){

            const QObjectList& children = this->children();

            const int sz = children.size();

            if (0 < sz){

                //beginStoreNode

                int cc;
                for (cc = 0; cc < sz; cc++){

                    Script* child = qobject_cast<Script*>(children.at(cc));

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
bool Scripts::drop(Script* script){
    if (script){

        const SystemScriptSymbol* linkSource = script->getLinkSource();
        if (linkSource){
            /*
             */
            script->setParent(0);
            script->deleteLater();
            /*
             */
            const QString* hostUuid = this->getHostUuid();
            /*
             */
            QSqlQuery op(*hcdb);
            /*
             * Delete everything
             */
            QString dsql("DELETE FROM ");
            dsql += HcdbScriptTableName;
            dsql += " WHERE HOST_UUID = '";
            dsql += *hostUuid;
            dsql += "' AND SOURCE = '";
            dsql += linkSource->toString();
            dsql += "';";

            return (op.prepare(dsql) && op.exec());
        }
    }
    return false;
}
bool Scripts::deconfigure(Script* script){
    if (script){

        const SystemScriptSymbol* linkSource = script->getLinkSource();
        if (linkSource){
            /*
             */
            script->setLinkTarget(0);
            /*
             */
            const QString* hostUuid = this->getHostUuid();

            QSqlQuery op(*hcdb);
            /*
             * Delete everything
             */
            QString dsql("UPDATE ");
            dsql += HcdbScriptTableName;
            dsql += " SET TARGET = NULL";
            dsql += " WHERE HOST_UUID = '";
            dsql += *hostUuid;
            dsql += "' AND SOURCE = '";
            dsql += linkSource->toString();
            dsql += "';";

            return (op.prepare(dsql) && op.exec());
        }
    }
    return false;
}
bool Scripts::done(int write){

    if (0 == write){

        return Scripts::write();
    }
    else {

        return false;
    }
}
const QString* Scripts::getHostUuid() const {

    HCDB* parent = qobject_cast<HCDB*>(this->parent());

    return parent->getHostUuid();
}
bool Scripts::insertObjectTreeList(){
    const QObjectList& children = this->children();
    const int index = children.size();
    if (0 == index){

        beginInsertNode(index,index);

        (new Script(this));

        endInsertNode();

        return true;
    }
    else {
        Script* last = static_cast<Script*>(children.last());

        if (last->isInert()){

            return false;
        }
        else {
            beginInsertNode(index,index);

            (new Script(this));

            endInsertNode();

            return true;
        }
    }
}
bool Scripts::removeObjectTreeList(int idx){
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
Script* Scripts::find(const SystemScriptSymbol& source){
    const QObjectList& children = this->children();
    const int size = children.size();
    int cc;
    for (cc = 0; cc < size; cc++){

        Script* child = dynamic_cast<Script*>(children.at(cc));
        if (child){

            const SystemScriptSymbol* childSource = child->getLinkSource();
            if (childSource && childSource->operator==(source)){

                return child;
            }
        }
    }
    return 0;
}
Script* Scripts::find(const QString& source){
    const QObjectList& children = this->children();
    const int size = children.size();
    int cc;
    for (cc = 0; cc < size; cc++){

        Script* child = dynamic_cast<Script*>(children.at(cc));
        if (child){

            const SystemScriptSymbol* childSource = child->getLinkSource();
            if (childSource && childSource->operator==(source)){

                return child;
            }
        }
    }
    return 0;
}
Script* Scripts::find(const char* source){
    const QObjectList& children = this->children();
    const int size = children.size();
    int cc;
    for (cc = 0; cc < size; cc++){

        Script* child = dynamic_cast<Script*>(children.at(cc));
        if (child){

            const SystemScriptSymbol* childSource = child->getLinkSource();
            if (childSource && childSource->operator==(source)){

                return child;
            }
        }
    }
    return 0;
}
