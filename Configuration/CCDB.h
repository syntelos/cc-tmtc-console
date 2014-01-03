/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_CCDB_H
#define _CONSOLE_CONFIGURATION_CCDB_H

#include <QMetaProperty>
#include <QObject>
#include <QScriptable>
#include <QSqlDatabase>
#include <QString>
#include <QWidget>

#include "Storage/StorageModel.h"
#include "System/SystemScriptable.h"

#define CcdbDriverType "QSQLITE"
#define CcdbDatabaseName "CCDB"
#define CcdbTableName "CCDB_TABLE"

/*!
 * The Console Configuration Data Base implements the \class
 * StorageModel 
 */
class CCDB : public StorageModel,
    public SystemScriptable
{
    Q_OBJECT;
    Q_PROPERTY(QString hostUuid READ getHostUuid USER true FINAL);
    Q_PROPERTY(QString hcdbDriverType READ getHcdbDriverType WRITE setHcdbDriverType USER true FINAL);
    Q_PROPERTY(QString hcdbDatabaseName READ getHcdbDatabaseName WRITE setHcdbDatabaseName USER true FINAL);
    Q_PROPERTY(QString hcdbDatabaseOptions READ getHcdbDatabaseOptions WRITE setHcdbDatabaseOptions USER true FINAL);
    Q_PROPERTY(QString hcdbHostName READ getHcdbHostName WRITE setHcdbHostName USER true FINAL);
    Q_PROPERTY(int     hcdbHostPort READ getHcdbHostPort WRITE setHcdbHostPort USER true FINAL);
    Q_PROPERTY(QString hcdbUserName READ getHcdbUserName WRITE setHcdbUserName USER true FINAL);
    Q_PROPERTY(QString hcdbUserPass READ getHcdbUserPass WRITE setHcdbUserPass USER true FINAL);


    QSqlDatabase* db;

    QString* hostUuid;
    QString* hcdbDriverType;
    QString* hcdbDatabaseName;
    QString* hcdbDatabaseOptions;
    QString* hcdbHostName;
    int      hcdbHostPort;
    QString* hcdbUserName;
    QString* hcdbUserPass;
    /*!
     * Create table and populate with default values
     */
    void init();

    void setHostUuid(QString& hostUuid);

 public:
    /*!
     * Open database and init or read.
     *
     * Throws "ErrorDatabase" for a failure to open its
     * database.
     */
    CCDB(QObject* parent);
    /*!
     * Close database.
     */
    ~CCDB();
    /*!
     * Have driver type and database name
     */
    bool isReady() const;
    /*!
     */
    const QString* getHostUuid() const;
    /*!
     */
    const QString* getHcdbDriverType() const;
    /*!
     */
    const QString* getHcdbDatabaseName() const;
    /*!
     */
    const QString* getHcdbDatabaseOptions() const;
    /*!
     */
    const QString* getHcdbHostName() const;
    /*!
     */
    int getHcdbHostPort() const;
    /*!
     */
    const QString* getHcdbUserName() const;
    /*!
     */
    const QString* getHcdbUserPass() const;
    /*!
     */
    void setHcdbDriverType(QString* driverType);
    /*!
     */
    void setHcdbDatabaseName(QString* databaseName);
    /*!
     */
    void setHcdbDatabaseOptions(QString* options);
    /*!
     */
    void setHcdbHostName(QString* hostname);
    /*!
     */
    void setHcdbHostPort(int portnum);
    /*!
     */
    void setHcdbUserName(QString* userName);
    /*!
     */
    void setHcdbUserPass(QString* userPass);

    /*!
     */
    void setHcdbDriverType(QString& driverType);
    /*!
     */
    void setHcdbDatabaseName(QString& databaseName);
    /*!
     */
    void setHcdbDatabaseOptions(QString& options);
    /*!
     */
    void setHcdbHostName(QString& hostname);
    /*!
     */
    void setHcdbUserName(QString& userName);
    /*!
     */
    void setHcdbUserPass(QString& userPass);
    /*!
     */
    virtual QWidget* createPropertyFormEditor(int index, const QMetaProperty& property);
    /*!
     */
    virtual QWidget* createPropertyFormLabel(int index, const QMetaProperty& property);
    /*!
     */
    virtual bool setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor);

 signals:
    /*!
     */
    void readSuccess();
    /*!
     */
    void readFailure();
    /*!
     */
    void writeSuccess();
    /*!
     */
    void writeFailure();

 public slots:
    /*!
     * Clear values
     */
    virtual void clear();
    /*!
     * Read table values
     */
    virtual bool read();
    /*!
     * Write table values
     */
    virtual bool write();
    /*!
     * Write for argument zero, otherwise ignore
     */
    virtual bool done(int write);


 private:
    Q_DISABLE_COPY(CCDB)
};

#endif
