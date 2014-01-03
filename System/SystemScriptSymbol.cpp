/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QMetaMethod>

#include "SystemScriptSymbol.h"

void SystemScriptSymbol::Init()
{
    static bool registered = false;
    if (!registered){
        registered = true;
        qRegisterMetaType<SystemScriptSymbol>("SystemScriptSymbol");
    }
}
SystemScriptSymbol::SystemScriptSymbol()
    : parsed(false)
{
    SystemScriptSymbol::Init();
}
SystemScriptSymbol::SystemScriptSymbol(const char* string)
    : QByteArray(string), parsed(false)
{
    SystemScriptSymbol::Init();

    qDebug() << "new SystemScriptSymbol(" << string << ")";
}
SystemScriptSymbol::SystemScriptSymbol(const QByteArray& string)
    : QByteArray(string), parsed(false)
{
    SystemScriptSymbol::Init();

    qDebug() << "new SystemScriptSymbol(" << string << ")";
}
SystemScriptSymbol::SystemScriptSymbol(const QString& string)
    : QByteArray(string.toUtf8()), parsed(false)
{
    SystemScriptSymbol::Init();

    qDebug() << "new SystemScriptSymbol(" << string << ")";
}
SystemScriptSymbol::SystemScriptSymbol(const QString& className, const QString& identifier)
    : QByteArray((className+"::"+identifier).toUtf8().data()), parsed(false)
{
    SystemScriptSymbol::Init();

    qDebug() << "new SystemScriptSymbol(" << this << ")";
}
SystemScriptSymbol::~SystemScriptSymbol(){
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
QString SystemScriptSymbol::toString() const {

    QString re(data());

    return re;
}
