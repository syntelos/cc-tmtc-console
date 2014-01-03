/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QLabel>
#include <QScriptEngine>
#include <QSqlQuery>
#include <QString>

#include "Library.h"
#include "Libraries.h"
#include "Configuration.h"
#include "ConfigurationError.h"

Q_DECLARE_METATYPE(Library*)

QScriptValue libraryToScriptValue(QScriptEngine *engine, Library* const &in){
    return engine->newQObject(in);
}

void libraryFromScriptValue(const QScriptValue &object, Library* &out){
    out = qobject_cast<Library*>(object.toQObject());
}


Library::Library(QObject* parent)
    : StorageListItem(parent), libraryUuid(vnul), fileIdentifier(vnul), languageClassName(vnul), connectionClassName(vnul)
{
    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), libraryToScriptValue, libraryFromScriptValue);
}
Library::Library(QSqlQuery& query, int start, QObject* parent)
    : StorageListItem(parent), fileIdentifier(vnul)
{
    this->read(query,start);

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), libraryToScriptValue, libraryFromScriptValue);
}
Library::Library(QSqlQuery& query, QObject* parent)
    : StorageListItem(parent), fileIdentifier(vnul)
{
    this->read(query);

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), libraryToScriptValue, libraryFromScriptValue);
}
Library::~Library(){

    if (libraryUuid){

        delete libraryUuid;
    }

    if (fileIdentifier){

        delete fileIdentifier;
    }

    if (languageClassName){

        delete languageClassName;
    }

    if (connectionClassName){

        delete connectionClassName;
    }

}
void Library::read(QSqlQuery& select, int start){

    QString s = select.value(start++).toString();

    this->setLibraryUuid(s);

    s = select.value(start++).toString();

    this->setFileIdentifier(s);

    s = select.value(start++).toString();

    this->setLanguageClassName(s);

    s = select.value(start++).toString();

    this->setConnectionClassName(s);

}
void Library::write(QSqlQuery& update, int start){


    if (this->libraryUuid)
        update.bindValue(start++,*this->libraryUuid);
    else
        update.bindValue(start++,QVariant(QVariant::String));

    if (this->fileIdentifier)
        update.bindValue(start++,*this->fileIdentifier);
    else
        update.bindValue(start++,QVariant(QVariant::String));

    if (this->languageClassName)
        update.bindValue(start++,*this->languageClassName);
    else
        update.bindValue(start++,QVariant(QVariant::String));

    if (this->connectionClassName)
        update.bindValue(start++,*this->connectionClassName);
    else
        update.bindValue(start++,QVariant(QVariant::String));

}
const QString* Library::getHostUuid() const {

    Libraries* parent = qobject_cast<Libraries*>(this->parent());

    return parent->getHostUuid();
}
const QString* Library::getLibraryUuid() const {

    return this->libraryUuid;
}
void Library::setLibraryUuid(QString* libraryUuid){
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
void Library::setLibraryUuid(QString& libraryUuid){
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
const QString* Library::getFileIdentifier() const {

    return this->fileIdentifier;
}
void Library::setFileIdentifier(QString* fileIdentifier){
    if (this->fileIdentifier){
        delete this->fileIdentifier;
    }

    if (fileIdentifier){

        if (0 < fileIdentifier->length()){

            this->fileIdentifier = fileIdentifier;

            this->setObjectName(*fileIdentifier);
        }
        else {
            delete fileIdentifier;

            this->fileIdentifier = vnul;

            QString empty;

            this->setObjectName(empty);
        }
    }
    else {

        this->fileIdentifier = vnul;

        QString empty;

        this->setObjectName(empty);
    }
    this->nodeDataChanged("fileIdentifier");
}
void Library::setFileIdentifier(QString& fileIdentifier){
    if (this->fileIdentifier){
        delete this->fileIdentifier;
    }

    if (0 < fileIdentifier.length()){

        this->fileIdentifier = new QString(fileIdentifier);

        this->setObjectName(fileIdentifier);
    }
    else {

        this->fileIdentifier = vnul;

        QString empty;

        this->setObjectName(empty);
    }
    this->nodeDataChanged("fileIdentifier");
}
const QString* Library::getLanguageClassName() const {

    return this->languageClassName;
}
void Library::setLanguageClassName(QString* languageClassName){
    if (this->languageClassName){
        delete this->languageClassName;
    }

    if (languageClassName){

        if (0 < languageClassName->length()){

            this->languageClassName = languageClassName;
        }
        else {
            delete languageClassName;

            this->languageClassName = vnul;
        }
    }
    else {

        this->languageClassName = vnul;
    }
}
void Library::setLanguageClassName(QString& languageClassName){
    if (this->languageClassName){
        delete this->languageClassName;
    }

    if (0 < languageClassName.length()){

        this->languageClassName = new QString(languageClassName);
    }
    else {

        this->languageClassName = vnul;
    }
}
const QString* Library::getConnectionClassName() const {

    return this->connectionClassName;
}
void Library::setConnectionClassName(QString* connectionClassName){
    if (this->connectionClassName){
        delete this->connectionClassName;
    }

    if (connectionClassName){

        if (0 < connectionClassName->length()){

            this->connectionClassName = connectionClassName;
        }
        else {
            delete connectionClassName;

            this->connectionClassName = vnul;
        }
    }
    else {

        this->connectionClassName = vnul;
    }
}
void Library::setConnectionClassName(QString& connectionClassName){
    if (this->connectionClassName){
        delete this->connectionClassName;
    }

    if (0 < connectionClassName.length()){

        this->connectionClassName = new QString(connectionClassName);
    }
    else {

        this->connectionClassName = vnul;
    }
}
QWidget* Library::createPropertyFormEditor(int index, const QMetaProperty& property){
    /*
     * TODO See QLibrary for requirements
     * TODO ?> File text edit & picker button [FileTextEdit : HBox] <?
     */
    return vnul;
}
QWidget* Library::createPropertyFormLabel(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("libraryUuid",propName)){

        return new QLabel("Library UUID");
    }
    else if ( 0 == strcmp("fileIdentifier",propName)){

        return new QLabel("Abstract Library Name");
    }
    else if ( 0 == strcmp("languageClassName",propName)){

        return new QLabel("TMTC Language Class Name");
    }
    else if ( 0 == strcmp("connectionClassName",propName)){

        return new QLabel("TMTC Connection Class Name");
    }
    else {
        /*
         * Accept default
         */
        return vnul;
    }
}
bool Library::setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor){

    return false;
}
bool Library::isInert(){

    return (vnul == fileIdentifier);
}
