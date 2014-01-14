/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QPlainTextEdit>
#include <QScriptEngine>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QUuid>
#include <QVariant>
#include <QWidget>

#include "Script.h"
#include "Scripts.h"
#include "Configuration.h"
#include "ConfigurationError.h"
#include "CodeEditor/CodeEditor.h"


QScriptValue scriptToScriptValue(QScriptEngine *engine, Script* const &in){
    return engine->newQObject(in);
}

void scriptFromScriptValue(const QScriptValue &object, Script* &out){
    out = qobject_cast<Script*>(object.toQObject());
}


Script::Script(QObject* parent)
    : StorageListItem(parent), linkSource(0), linkTarget(0), file(0), content(0)
{
    SystemScriptSymbol::Init();

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), scriptToScriptValue, scriptFromScriptValue);
}
Script::Script(QSqlQuery& query, int start, QObject* parent)
    : StorageListItem(parent), linkSource(0), linkTarget(0), file(0), content(0)
{
    SystemScriptSymbol::Init();

    this->read(query,start);

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), scriptToScriptValue, scriptFromScriptValue);
}
Script::Script(QSqlQuery& query, QObject* parent)
    : StorageListItem(parent), linkSource(0), linkTarget(0), file(0), content(0)
{
    SystemScriptSymbol::Init();

    this->read(query);

    qScriptRegisterMetaType(Configuration::Instance()->getScriptEngine(), scriptToScriptValue, scriptFromScriptValue);
}
Script::~Script(){

    if (linkSource){

        delete linkSource;
    }

    if (linkTarget){

        delete linkTarget;
    }

    if (file){

        delete file;
    }

    if (content){

        delete content;
    }
}
void Script::read(QSqlQuery& select, int start){

    SystemScriptSymbol s0(select.value(start++).toString());

    qDebug() << "Script read symbol source " << s0.toString();

    this->setLinkSource(s0);

    SystemScriptSymbol s1(select.value(start++).toString());

    qDebug() << "Script read symbol target " << s1.toString();

    this->setLinkTarget(s1);

    QString s2 = select.value(start++).toString();

    this->setFile(s2);

    QString s3 = select.value(start++).toString();

    this->setContent(s3);

}
void Script::write(QSqlQuery& insert, int start){


    if (linkSource){

        qDebug() << "Script write symbol source " << linkSource->toString();

        insert.bindValue(start++,linkSource->toString());
    }
    else
        insert.bindValue(start++,QVariant(QVariant::String));


    if (linkTarget){

        qDebug() << "Script write symbol target " << linkTarget->toString();

        insert.bindValue(start++,linkTarget->toString());
    }
    else
        insert.bindValue(start++,QVariant(QVariant::String));


    if (file)
        insert.bindValue(start++,*file);
    else
        insert.bindValue(start++,QVariant(QVariant::String));


    if (content)
        insert.bindValue(start++,*content);
    else
        insert.bindValue(start++,QVariant(QVariant::String));


}
const QString* Script::getHostUuid() const {

    Scripts* parent = qobject_cast<Scripts*>(this->parent());

    return parent->getHostUuid();
}
const SystemScriptSymbol* Script::getLinkSource() const {

    return this->linkSource;
}
void Script::setLinkSource(SystemScriptSymbol* linkSource){
    if (this->linkSource){
        delete this->linkSource;
    }

    if (linkSource && linkSource->isNotInert()){

        if (0 < linkSource->length()){

            this->linkSource = linkSource;

            setObjectName(linkSource->toString());
        }
        else {
            delete linkSource;

            this->linkSource = 0;

            setObjectName("");
        }
    }
    else {
        this->linkSource = 0;

        setObjectName("");
    }
    this->nodeDataChanged("linkSource");
}
void Script::setLinkSource(SystemScriptSymbol& linkSource){

    if (this->linkSource){
        delete this->linkSource;
    }

    if (linkSource.isNotInert()){

        this->linkSource = new SystemScriptSymbol(linkSource);

        setObjectName(linkSource.toString());
    }
    else {

        this->linkSource = 0;

        setObjectName(linkSource);
    }
    this->nodeDataChanged("linkSource");
}
void Script::setLinkSource(QString& linkSource){

    if (this->linkSource){
        delete this->linkSource;
    }

    if (0 < linkSource.length()){

        this->linkSource = new SystemScriptSymbol(linkSource.toUtf8());

        setObjectName(linkSource);
    }
    else {

        this->linkSource = 0;

        setObjectName("");
    }
    this->nodeDataChanged("linkSource");
}
const SystemScriptSymbol* Script::getLinkTarget() const {

    return this->linkTarget;
}
void Script::setLinkTarget(SystemScriptSymbol* linkTarget){
    if (this->linkTarget){
        delete this->linkTarget;
    }

    if (linkTarget){

        if (linkTarget->isNotInert()){

            this->linkTarget = linkTarget;
        }
        else {
            delete linkTarget;

            this->linkTarget = 0;
        }
    }
    else {

        this->linkTarget = 0;
    }
}
void Script::setLinkTarget(SystemScriptSymbol& linkTarget){
    if (this->linkTarget){
        delete this->linkTarget;
    }

    if (linkTarget.isNotInert()){

        this->linkTarget = new SystemScriptSymbol(linkTarget);
    }
    else {

        this->linkTarget = 0;
    }
}
void Script::setLinkTarget(QString& linkTarget){

    if (this->linkTarget){
        delete this->linkTarget;
    }

    if (0 < linkTarget.length()){

        this->linkTarget = new SystemScriptSymbol(linkTarget.toUtf8());
    }
    else {

        this->linkTarget = 0;
    }
}
const QString* Script::getFile() const {

    return this->file;
}
void Script::setFile(QString* file){
    if (this->file){
        delete this->file;
    }

    if (file){

        this->file = file;
    }
    else {

        this->file = 0;
    }
}
void Script::setFile(QString& file){
    if (this->file){
        delete this->file;
    }

    if (0 < file.length()){

        this->file = new QString(file);
    }
    else {

        this->file = 0;
    }
}
void Script::setFile(QFileInfo& file){
    if (this->file){
        delete this->file;
    }

    if (file.exists()){

        this->file = new QString(file.filePath());
    }
    else {

        this->file = 0;
    }
}
const QString* Script::getContent() const {

    return this->content;
}
void Script::setContent(QString* content){
    if (this->content){
        delete this->content;
    }

    if (content){

        if (0 < content->length()){

            this->content = content;
        }
        else {
            delete content;

            this->content = 0;
        }
    }
    else {

        this->content = 0;
    }
}
void Script::setContent(QString& content){
    if (this->content){
        delete this->content;
    }

    if (0 < content.length()){

        this->content = new QString(content);
    }
    else {

        this->content = 0;
    }
}
void Script::setContent(QByteArray& content){
    if (this->content){
        delete this->content;
    }

    if (0 < content.length()){

        this->content = new QString(content.data());
    }
    else {

        this->content = 0;
    }
}
QWidget* Script::createPropertyFormEditor(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("linkSource",propName)){

        QLineEdit* editor = new QLineEdit();

        if (this->linkSource && 0 < this->linkSource->length()){

            editor->setProperty("text",this->linkSource->constData());
        }
        editor->setProperty("maxLength",40);
        return editor;
    }
    else if ( 0 == strcmp("linkTarget",propName)){

        QLineEdit* editor = new QLineEdit();

        if (this->linkTarget && 0 < this->linkTarget->length()){

            editor->setProperty("text",this->linkTarget->constData());
        }
        editor->setProperty("maxLength",40);
        return editor;
    }
    else if ( 0 == strcmp("file",propName)){

        QLineEdit* editor = new QLineEdit();

        if (this->file){

            editor->setProperty("text",*this->file);
        }
        editor->setProperty("maxLength",40);
        return editor;
    }
    else if ( 0 == strcmp("content",propName)){

        CodeEditor* editor = new CodeEditor();

        if (this->content){

            editor->setPlainText(*this->content);
        }
        return editor;
    }
    else {
        /*
         * Accept default
         */
        return 0;
    }
}
QWidget* Script::createPropertyFormLabel(int index, const QMetaProperty& property){

    const char* propName = property.name();

    if ( 0 == strcmp("linkSource",propName)){

        return new QLabel("LinkSource");
    }
    else if ( 0 == strcmp("linkTarget",propName)){

        return new QLabel("LinkTarget");
    }
    else if ( 0 == strcmp("file",propName)){

        return new QLabel("File");
    }
    else if ( 0 == strcmp("content",propName)){

        return new QLabel("Content");
    }
    else {
        /*
         * Accept default
         */
        return 0;
    }
}
bool Script::setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor){

    const char* name = property.name();

    if ( 0 == strcmp("linkSource",name)){

        QString value = editor.property("text").toString();

        this->setLinkSource(value);

        return true;
    }
    else if ( 0 == strcmp("linkTarget",name)){

        QString value = editor.property("text").toString();

        this->setLinkTarget(value);

        return true;
    }
    else if ( 0 == strcmp("content",name)){

        QString value = editor.property("plainText").toString();

        this->setContent(value);

        return true;
    }
    else {

        return false;
    }
}
bool Script::isInert(){

    return (0 == linkSource || 0 == linkTarget);
}
void Script::importToObjectTreeNode(){

    QString dir;
    
    if (this->file){

        QFileInfo file(*this->file);
        QDir fileDir = file.dir();
        dir.append(fileDir.path());
    }
    else {

        dir.append(qApp->applicationDirPath());
    }

    QFileDialog dialog(ancestorWidget(),"Import JS",dir,"JavaScript files (*.js)");

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);

    if (dialog.exec()){

        QStringList list = dialog.selectedFiles();
        if (1 == list.size()){

            QFile file(list.at(0));

            if (file.open(QIODevice::ReadOnly)){

                /*
                 */
                QByteArray content = file.readAll();

                this->setContent(content);

                nodeDataChanged("content");

                /*
                 */
                QFileInfo info(file);

                this->setFile(info);

                nodeDataChanged("file");
            }
            file.close();
        }
    }
}
void Script::exportFromObjectTreeNode(){

    QString* content = this->content;

    QString dir;
    
    if (this->file){

        QFileInfo file(*this->file);
        QDir fileDir = file.dir();
        dir.append(fileDir.path());
    }
    else {

        dir.append(qApp->applicationDirPath());
    }

    QFileDialog dialog(ancestorWidget(),"Export JS",dir,"JavaScript file (*.js)");

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (dialog.exec()){

        QStringList list = dialog.selectedFiles();
        if (1 == list.size()){

            QFile file(list.at(0));

            if (file.open(QIODevice::WriteOnly)){

                if (content){

                    file.write(content->toUtf8());
                }
            }
            file.close();
        }
    }
}
