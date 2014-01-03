/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QComboBox>
#include <QFSFileEngine>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QScriptEngine>
#include <QString>
#include <QStringList>
#include <QUuid>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "ApplicationUUID.h"
#include "Configuration.h"
#include "ConfigurationError.h"
#include "CCDB.h"
#include "Null.h"

Q_DECLARE_METATYPE(CCDB*)

QScriptValue ccdbToScriptValue(QScriptEngine *engine, CCDB* const &in){
    return engine->newQObject(in);
}

void ccdbFromScriptValue(const QScriptValue &object, CCDB* &out){
    out = qobject_cast<CCDB*>(object.toQObject());
}


CCDB::CCDB(QObject* parent)
    : StorageModel(parent),
      db(new QSqlDatabase(QSqlDatabase::addDatabase(CcdbDriverType,CcdbTableName))),
      hostUuid(vnul),
      hcdbDriverType(vnul),
      hcdbDatabaseName(vnul),
      hcdbDatabaseOptions(vnul),
      hcdbHostName(vnul),
      hcdbHostPort(0),
      hcdbUserName(vnul),
      hcdbUserPass(vnul)
{
    initSystemScriptable(this);

    if (db->isValid()){

        db->setDatabaseName(CcdbDatabaseName);

        if (db->open()){

            if (!read()){

                init();
            }
        }
        else {
            throw ConfigurationError::DatabaseOpen("CCDB");
        }
    }
    else {
        throw ConfigurationError::DatabaseDriver("CCDB");
    }
    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), ccdbToScriptValue, ccdbFromScriptValue);
}
CCDB::~CCDB(){

    delete db;

    if (hcdbDriverType){
        delete hcdbDriverType;
    }

    if (hcdbDatabaseName){
        delete hcdbDatabaseName;
    }

    if (hcdbDatabaseOptions){
        delete hcdbDatabaseOptions;
    }

    if (hcdbHostName){
        delete hcdbHostName;
    }

    if (hcdbUserName){
        delete hcdbUserName;
    }

    if (hcdbUserPass){
        delete hcdbUserPass;
    }
}
void CCDB::init(){

    /*
     * DROP
     */
    QString dsql("DROP TABLE ");
    dsql += CcdbTableName;
    dsql += ";";

    QSqlQuery drop(dsql,*db);

    bool dropExec = drop.exec();
    /*
     * CREATE
     */
    QString csql("CREATE TABLE ");
    csql += CcdbTableName;
    /*
     * There is only one record in this table per the combination of
     * APP_UUID and HOST_UUID.  While it's unlikely, it is permissable
     * for a single CCDB database to be employed for distinct
     * applications of this library.
     */
    csql += " ( APP_UUID CHARACTER(38) UNIQUE, HOST_UUID CHARACTER(38) UNIQUE, DRIVER VARCHAR, NAME VARCHAR, OPTIONS VARCHAR, HOST VARCHAR, PORT INTEGER, USER VARCHAR, USER_PASS VARCHAR );";

    QSqlQuery create(csql,*db);

    bool createExec = create.exec();
    /*
     * INSERT
     */
    QUuid hostUuid = QUuid::createUuid();

    QString hostUuidString = hostUuid.toString();


    QString isql("INSERT INTO ");
    isql += CcdbTableName;
    isql += " ( APP_UUID, HOST_UUID, PORT ) VALUES ( '";
    isql += ApplicationUUID;
    isql += "', '";
    isql += hostUuidString;
    isql += "', 0 );";

    QSqlQuery insert(isql,*db);

    bool insertExec = insert.exec();

    this->setHostUuid(hostUuidString);

}
void CCDB::clear(){
    setHcdbDriverType(vnul);
    setHcdbDatabaseName(vnul);
    setHcdbDatabaseOptions(vnul);
    setHcdbHostName(vnul);
    setHcdbHostPort(0);
    setHcdbUserName(vnul);
    setHcdbUserPass(vnul);
}
bool CCDB::read(){

    QString sql = "SELECT HOST_UUID, DRIVER, NAME, OPTIONS, HOST, PORT, USER, USER_PASS FROM ";
    sql += CcdbTableName;
    sql += " WHERE APP_UUID = '";
    sql += ApplicationUUID;
    sql += "';";

    QSqlQuery select(sql,*db);

    bool selectExec = select.exec();

    bool selectNext = select.next();

    if (selectExec && selectNext){
        qDebug() << "CCDB read select success [exec " << selectExec << "] [next " << selectNext << "]";

        QString s = select.value(0).toString();

        CCDB::setHostUuid(s);

        s = select.value(1).toString();

        CCDB::setHcdbDriverType(s);

        s = select.value(2).toString();

        CCDB::setHcdbDatabaseName(s);

        s = select.value(3).toString();

        CCDB::setHcdbDatabaseOptions(s);

        s = select.value(4).toString();

        CCDB::setHcdbHostName(s);

        CCDB::setHcdbHostPort(select.value(5).toInt());

        s = select.value(6).toString();

        CCDB::setHcdbUserName(s);

        s = select.value(7).toString();

        CCDB::setHcdbUserPass(s);

        emit readSuccess();

        return true;
    }
    else {
        qDebug() << "CCDB read select failure [exec " << selectExec << "] [next " << selectNext << "]";

        emit readFailure();

        return false;
    }
}
bool CCDB::write(){
    QString sql("UPDATE ");
    sql += CcdbTableName;
    sql += " SET DRIVER = ?, NAME = ?, OPTIONS = ?, HOST = ?, PORT = ?, USER = ?, USER_PASS = ? WHERE APP_UUID = '";
    sql += ApplicationUUID;
    sql += "';";

    QSqlQuery update(sql,*db);

    if (hcdbDriverType)
        update.bindValue(0,*hcdbDriverType);
    else
        update.bindValue(0,"QSQLITE");

    if (hcdbDatabaseName)
        update.bindValue(1,*hcdbDatabaseName);
    else
        update.bindValue(1,"HCDB");

    if (hcdbDatabaseOptions)
        update.bindValue(2,*hcdbDatabaseOptions);
    else
        update.bindValue(2,QVariant(QVariant::String));

    if (hcdbHostName)
        update.bindValue(3,*hcdbHostName);
    else
        update.bindValue(3,QVariant(QVariant::String));

    update.bindValue(4, hcdbHostPort);

    if (hcdbUserName)
        update.bindValue(5,*hcdbUserName);
    else
        update.bindValue(5,QVariant(QVariant::String));

    if (hcdbUserPass)
        update.bindValue(6,*hcdbUserPass);
    else
        update.bindValue(6,QVariant(QVariant::String));


    if (update.exec()){

        qDebug() << "CCDB write update success";

        emit writeSuccess();

        return true;
    }
    else {
        qDebug() << "CCDB write update failure";

        emit writeFailure();

        return false;
    }
}
bool CCDB::done(int write){

    if (0 == write){

        return CCDB::write();
    }
    else {

        return false;
    }
}

bool CCDB::isReady() const {

    return (hostUuid && hcdbDriverType && hcdbDatabaseName);
}
const QString* CCDB::getHostUuid() const {

    return hostUuid;
}
/* (private)
 */
void CCDB::setHostUuid(QString& hostUuid){

    if (this->hostUuid){
        if (*this->hostUuid == hostUuid){

            qDebug() << "CCDB setHostUuid " << hostUuid << " identical";

            return;
        }
        else {
            qDebug() << "CCDB setHostUuid " << hostUuid << " delete";

            delete this->hostUuid;
        }
    }
    else {
        qDebug() << "CCDB setHostUuid " << hostUuid;
    }

    if (0 < hostUuid.length()){

        this->hostUuid = new QString(hostUuid);
    }
    else {

        this->hostUuid = vnul;
    }
}
const QString* CCDB::getHcdbDriverType() const {

    return hcdbDriverType;
}
const QString* CCDB::getHcdbDatabaseName() const {

    return hcdbDatabaseName;
}
const QString* CCDB::getHcdbDatabaseOptions() const {

    return hcdbDatabaseOptions;
}
const QString* CCDB::getHcdbHostName() const {

    return hcdbHostName;
}
int CCDB::getHcdbHostPort() const {

    return hcdbHostPort;
}
const QString* CCDB::getHcdbUserName() const {

    return hcdbUserName;
}
const QString* CCDB::getHcdbUserPass() const {

    return hcdbUserPass;
}

void CCDB::setHcdbDriverType(QString* driverType){
    if (this->hcdbDriverType){
        delete this->hcdbDriverType;
    }

    if (driverType){

        if (0 < driverType->length()){

            this->hcdbDriverType = driverType;
        }
        else {
            delete driverType;

            this->hcdbDriverType = vnul;
        }
    }
    else {

        this->hcdbDriverType = vnul;
    }
}
void CCDB::setHcdbDatabaseName(QString* databaseName){
    if (this->hcdbDatabaseName){
        delete this->hcdbDatabaseName;
    }

    if (databaseName){

        if (0 < databaseName->length()){

            this->hcdbDatabaseName = databaseName;
        }
        else {
            delete databaseName;

            this->hcdbDatabaseName = vnul;
        }
    }
    else {

        this->hcdbDatabaseName = vnul;
    }
}
void CCDB::setHcdbDatabaseOptions(QString* databaseOptions){
    if (this->hcdbDatabaseOptions){
        delete this->hcdbDatabaseOptions;
    }

    if (databaseOptions){

        if (0 < databaseOptions->length()){

            this->hcdbDatabaseOptions = databaseOptions;
        }
        else {
            delete databaseOptions;

            this->hcdbDatabaseOptions = vnul;
        }
    }
    else {

        this->hcdbDatabaseOptions = vnul;
    }
}
void CCDB::setHcdbHostName(QString* hostName){
    if (this->hcdbHostName){
        delete this->hcdbHostName;
    }

    if (hostName){

        if (0 < hostName->length()){

            this->hcdbHostName = hostName;
        }
        else {
            delete hostName;

            this->hcdbHostName = vnul;
        }
    }
    else {

        hcdbHostName = vnul;
    }
}
void CCDB::setHcdbHostPort(int portnum){

    if (0 < portnum){

        hcdbHostPort = portnum;
    }
    else {

        hcdbHostPort = 0;
    }
}
void CCDB::setHcdbUserName(QString* userName){
    if (this->hcdbUserName){
        delete this->hcdbUserName;
    }

    if (userName){

        if (0 < userName->length()){

            this->hcdbUserName = userName;
        }
        else {
            delete userName;

            this->hcdbUserName = vnul;
        }
    }
    else {

        hcdbUserName = vnul;
    }
}
void CCDB::setHcdbUserPass(QString* userPass){
    if (this->hcdbUserPass){
        delete this->hcdbUserPass;
    }

    if (userPass){

        if (0 < userPass->length()){

            this->hcdbUserPass = userPass;
        }
        else {
            delete userPass;

            this->hcdbUserPass = vnul;
        }
    }
    else {

        this->hcdbUserPass = vnul;
    }
}

void CCDB::setHcdbDriverType(QString& driverType){
    if (this->hcdbDriverType){
        delete this->hcdbDriverType;
    }

    if (0 < driverType.length()){

        this->hcdbDriverType = new QString(driverType);
    }
    else {

        this->hcdbDriverType = vnul;
    }
}
void CCDB::setHcdbDatabaseName(QString& databaseName){
    if (this->hcdbDatabaseName){
        delete this->hcdbDatabaseName;
    }

    if (0 < databaseName.length()){

        this->hcdbDatabaseName = new QString(databaseName);
    }
    else {

        this->hcdbDatabaseName = vnul;
    }
}
void CCDB::setHcdbDatabaseOptions(QString& databaseOptions){
    if (this->hcdbDatabaseOptions){
        delete this->hcdbDatabaseOptions;
    }

    if (0 < databaseOptions.length()){

        this->hcdbDatabaseOptions = new QString(databaseOptions);
    }
    else {

        this->hcdbDatabaseOptions = vnul;
    }
}
void CCDB::setHcdbHostName(QString& hostName){
    if (this->hcdbHostName){
        delete this->hcdbHostName;
    }

    if (0 < hostName.length()){

        this->hcdbHostName = new QString(hostName);
    }
    else {

        this->hcdbHostName = vnul;
    }
}
void CCDB::setHcdbUserName(QString& userName){
    if (this->hcdbUserName){
        delete this->hcdbUserName;
    }

    if (0 < userName.length()){

        this->hcdbUserName = new QString(userName);
    }
    else {

        this->hcdbUserName = vnul;
    }
}
void CCDB::setHcdbUserPass(QString& userPass){
    if (this->hcdbUserPass){
        delete this->hcdbUserPass;
    }

    if (0 < userPass.length()){

        this->hcdbUserPass = new QString(userPass);
    }
    else {

        this->hcdbUserPass = vnul;
    }
}
QWidget* CCDB::createPropertyFormEditor(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("hcdbDriverType",propName)){
        /*
         * [TODO] "SqlDatabaseComboBox" 
         * Use (QStringList QSqlDatabase::drivers) 
         * to create a "SqlDatabaseComboBox" that
         * matches available drivers with their 
         * ui text representation.
         * [/TODO]
         */
        QComboBox* editor = new QComboBox();

        editor->addItem("DB2", "QDB2");           // 0
        editor->addItem("Borland", "QIBASE");     // 1
        editor->addItem("MySQL", "QMYSQL");       // 2
        editor->addItem("Oracle", "QOCI");        // 3
        editor->addItem("ODBC", "QODBC");         // 4
        editor->addItem("PostgreSQL", "QPSQL");   // 5
        editor->addItem("SQLite 3+", "QSQLITE");  // 6
        editor->addItem("SQLite 2", "QSQLITE2");  // 7
        editor->addItem("Sybase", "QTDS");        // 8

        editor->setEditable(false);

        if (!hcdbDriverType){

            editor->setCurrentIndex(6); // SQLITE 3
        }
        else {

            int idx = editor->findData(*hcdbDriverType);

            if (-1 < idx){

                editor->setCurrentIndex(idx);
            }
        }
        return editor;
    }
    else if ( 0 == strcmp("hcdbDatabaseName",propName)){

        QLineEdit* editor = vnul;

        if (hcdbDatabaseName)
            editor = new QLineEdit(*hcdbDatabaseName);
        else
            editor = new QLineEdit("HCDB"); // Inject GUI default value

        editor->setMaxLength(40);
        return editor;
    }
    else if ( 0 == strcmp("hcdbUserPass",propName)){

        QLineEdit* editor = vnul;

        if (hcdbUserPass)
            editor = new QLineEdit(*hcdbUserPass);
        else
            editor = new QLineEdit();

        editor->setEchoMode(QLineEdit::Password);
        editor->setMaxLength(40);
        return editor;
    }
    else {
        /*
         * Accept default
         */
        return vnul;
    }
}
QWidget* CCDB::createPropertyFormLabel(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("hostUuid",propName)){

        return new QLabel("Host UUID");
    }
    else if ( 0 == strcmp("hcdbDriverType",propName)){

        return new QLabel("Driver");
    }
    else if ( 0 == strcmp("hcdbDatabaseName",propName)){

        return new QLabel("Database Name");
    }
    else if ( 0 == strcmp("hcdbDatabaseOptions",propName)){

        return new QLabel("Driver Options");
    }
    else if ( 0 == strcmp("hcdbHostName",propName)){

        return new QLabel("Host Name");
    }
    else if ( 0 == strcmp("hcdbHostPort",propName)){

        return new QLabel("Port Number");
    }
    else if ( 0 == strcmp("hcdbUserName",propName)){

        return new QLabel("User Name");
    }
    else if ( 0 == strcmp("hcdbUserPass",propName)){

        return new QLabel("User Password");
    }
    else {
        /*
         * Accept default
         */
        return vnul;
    }
}
bool CCDB::setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor){

    return false;
}
