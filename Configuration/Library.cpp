/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QLabel>
#include <QString>

#include "Library.h"
#include "Libraries.h"


QScriptValue libraryToScriptValue(QScriptEngine *engine, Library* const &in){
    return engine->newQObject(in);
}

void libraryFromScriptValue(const QScriptValue &object, Library* &out){
    out = qobject_cast<Library*>(object.toQObject());
}

void Library::InitScriptMetaType(QScriptEngine* engine){
    qScriptRegisterMetaType(engine, libraryToScriptValue, libraryFromScriptValue);
}
Library::Library(QObject* parent)
    : ObjectTreeNode(parent), libraryUuid(0), fileIdentifier(0), languageClassName(0), connectionClassName(0)
{
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

            this->libraryUuid = 0;
        }
    }
    else {

        this->libraryUuid = 0;
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

        this->libraryUuid = 0;
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

            this->fileIdentifier = 0;

            QString empty;

            this->setObjectName(empty);
        }
    }
    else {

        this->fileIdentifier = 0;

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

        this->fileIdentifier = 0;

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

            this->languageClassName = 0;
        }
    }
    else {

        this->languageClassName = 0;
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

        this->languageClassName = 0;
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

            this->connectionClassName = 0;
        }
    }
    else {

        this->connectionClassName = 0;
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

        this->connectionClassName = 0;
    }
}
QWidget* Library::createPropertyFormEditor(int index, const QMetaProperty& property){
    /*
     * TODO See QLibrary for requirements
     * TODO ?> File text edit & picker button [FileTextEdit : HBox] <?
     */
    return 0;
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

        return new QLabel("System Language Class Name");
    }
    else if ( 0 == strcmp("connectionClassName",propName)){

        return new QLabel("System Connection Class Name");
    }
    else {
        /*
         * Accept default
         */
        return 0;
    }
}
bool Library::setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor){

    return false;
}
bool Library::isInert(){

    return (0 == fileIdentifier);
}
