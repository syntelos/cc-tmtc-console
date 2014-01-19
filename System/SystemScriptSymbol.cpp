/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QMetaMethod>

#include "SystemScriptSymbol.h"

const int SystemScriptSymbol::InitMetaTypeId = qRegisterMetaType<SystemScriptSymbol>("SystemScriptSymbol");

SystemScriptSymbol::SystemScriptSymbol()
    : QByteArray(), prefix(), suffix(), parsed(false)
{
}
SystemScriptSymbol::SystemScriptSymbol(const SystemScriptSymbol& copy)
    : QByteArray(copy), prefix(copy.prefix), suffix(copy.suffix), parsed(copy.parsed)
{
}
SystemScriptSymbol::SystemScriptSymbol(const char* string)
    : QByteArray(string), prefix(), suffix(), parsed(false)
{
}
SystemScriptSymbol::SystemScriptSymbol(const char* string, int len)
    : QByteArray(string,len), prefix(), suffix(), parsed(false)
{
}
SystemScriptSymbol::SystemScriptSymbol(const QByteArray& string)
    : QByteArray(string), prefix(), suffix(), parsed(false)
{
}
SystemScriptSymbol::SystemScriptSymbol(const QString& string)
    : QByteArray(string.toUtf8().data()), prefix(), suffix(), parsed(false)
{
}
SystemScriptSymbol::SystemScriptSymbol(const QString& className, const QString& identifier)
    : QByteArray((className+"::"+identifier).toUtf8().data()), prefix(), suffix(), parsed(false)
{
}
SystemScriptSymbol::~SystemScriptSymbol(){
}
void SystemScriptSymbol::clear(){

    QByteArray::clear();
    parsed = false;
    prefix.clear();
    suffix.clear();
}
void SystemScriptSymbol::set(const QString& string){

    clear();

    QByteArray::append(string);

}
void SystemScriptSymbol::set(const QByteArray& string){

    clear();

    QByteArray::append(string);

}
void SystemScriptSymbol::set(const char *string){

    clear();

    QByteArray::append(string);

}
void SystemScriptSymbol::set(const char *string, int len){

    clear();

    QByteArray::append(string,len);

}
bool SystemScriptSymbol::isInert() const {

    return this->isEmpty();
}
bool SystemScriptSymbol::isNotInert() const {

    return (!this->isEmpty());
}
void SystemScriptSymbol::parse() const {
    if (!parsed){
        parsed = true;

        int idx = this->indexOf("::");

        if (-1 < idx){
            this->prefix.append(this->mid(0,idx));
            this->suffix.append(this->mid(idx+2));
        }
        else {
            this->suffix.append(this->data());
        }
    }
}
const QString& SystemScriptSymbol::getPrefix() const {
    this->parse();
    return this->prefix;
}
const char* SystemScriptSymbol::getPrefixSymbolic() const {
    this->parse();
    return this->prefix.toUtf8().constData();
}
const QString& SystemScriptSymbol::getSuffix() const {
    this->parse();
    return this->suffix;
}
const char* SystemScriptSymbol::getSuffixSymbolic() const {
    this->parse();
    return this->suffix.toUtf8().constData();
}
bool SystemScriptSymbol::operator==(const QObject* type) const {
    if (type){
        const QMetaObject* meta = type->metaObject();

        this->parse();

        if (0 < this->prefix.length()){

            return (this->prefix == meta->className());
        }
    }
    return false;
}
bool SystemScriptSymbol::operator==(const QMetaObject* type) const {
    this->parse();

    if (0 < this->prefix.length()){

        return (this->prefix == type->className());
    }
    else {

        return false;
    }
}
bool SystemScriptSymbol::operator==(const SystemScriptSymbol* that) const {
    this->parse();

    if (0 < this->prefix.length()){

        if (this->prefix != that->prefix){

            return false;
        }
        else {

            return (this->suffix == that->suffix);
        }
    }
    else {

        return false;
    }
}
bool SystemScriptSymbol::operator==(const SystemScriptSymbol& that) const {
    this->parse();

    if (0 < this->prefix.length()){

        if (this->prefix != that.prefix){

            return false;
        }
        else {

            return (this->suffix == that.suffix);
        }
    }
    else {

        return false;
    }
}
bool SystemScriptSymbol::operator==(const char* that) const {
    QString string(that);

    return QByteArray::operator==(string);
}
bool SystemScriptSymbol::operator==(const QString& that) const {

    return QByteArray::operator==(that);
}
bool SystemScriptSymbol::operator!=(const QObject* type) const {
    if (type){
        const QMetaObject* meta = type->metaObject();

        this->parse();

        if (0 < this->prefix.length()){

            return (this->prefix != meta->className());
        }
    }
    return false;
}
bool SystemScriptSymbol::operator!=(const QMetaObject* type) const {
    this->parse();

    if (0 < this->prefix.length()){

        return (this->prefix != type->className());
    }
    else {

        return false;
    }
}
bool SystemScriptSymbol::operator!=(const SystemScriptSymbol* that) const {
    this->parse();

    if (0 < this->prefix.length()){

        if (this->prefix != that->prefix){

            return true;
        }
        else {

            return (this->suffix != that->suffix);
        }
    }
    else {

        return false;
    }
}
bool SystemScriptSymbol::operator!=(const SystemScriptSymbol& that) const {
    this->parse();

    if (0 < this->prefix.length()){

        if (this->prefix != that.prefix){

            return true;
        }
        else {

            return (this->suffix != that.suffix);
        }
    }
    else {

        return false;
    }
}
bool SystemScriptSymbol::operator!=(const char* that) const {
    QString string(that);

    return QByteArray::operator!=(string);
}
bool SystemScriptSymbol::operator!=(const QString& that) const {

    return QByteArray::operator!=(that);
}
const char* SystemScriptSymbol::signal(const QMetaObject* type) const {

    const char* name = this->getSuffixSymbolic();
    const uint namelen = qstrlen(name);

    const int size = type->methodCount();
    const int mofs = type->methodOffset();

    int cc, signum = 0;

    for (cc = mofs; cc < size; cc++){

        QMetaMethod method = type->method(cc);

        if (QMetaMethod::Signal == method.methodType()){

            const char* signature = method.signature();

            const uint test = strspn(signature,name);

            if (namelen <= test){

                QString signal = QString("%1%2").arg(signum).arg(signature);

                return signal.toUtf8().constData();
            }
            else {
                signum += 1;
            }
        }
    }
    return NULL;
}
const char* SystemScriptSymbol::slot(const QMetaObject* type) const {

    const char* name = this->getSuffixSymbolic();
    const uint namelen = qstrlen(name);

    const int size = type->methodCount();
    const int mofs = type->methodOffset();

    int cc, signum = 0;

    for (cc = mofs; cc < size; cc++){

        QMetaMethod method = type->method(cc);

        if (QMetaMethod::Slot == method.methodType()){

            const char* signature = method.signature();

            const uint test = strspn(signature,name);

            if (namelen <= test){

                QString slot = QString("%1%2").arg(signum).arg(signature);

                return slot.toUtf8().constData();
            }
            else {
                signum += 1;
            }
        }
    }
    return NULL;
}
QString SystemScriptSymbol::toString() const {

    QString re(data());

    return re;
}
