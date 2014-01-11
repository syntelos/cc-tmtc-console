/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QMetaType>
#include <QStringList>

#include "SystemDeviceIdentifier.h"


const int SystemDeviceIdentifier::InitMetaTypeId = qRegisterMetaType<SystemDeviceIdentifier>("SystemDeviceIdentifier");

const SystemDeviceIdentifier* SystemDeviceIdentifier::BroadcastIdentifier = new SystemDeviceIdentifier();

QString SystemDeviceIdentifier::Cat(const QString& id, const quint16 defaultSuffix){
    if (0 < id.indexOf(':'))
        return id;
    else {
        QString re("%1:%2");
        re.arg(id).arg(defaultSuffix);
        return re;
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier()
    : QVariant(), suffix(0)
{
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const QString& id, const quint16 defaultSuffix)
    : QVariant(Cat(id,defaultSuffix)), suffix(0)
{
    QStringList idlist = id.split(':',QString::SkipEmptyParts);
    if (2 == idlist.length()){

        this->prefix += idlist.at(0);

        bool ok;
        ushort test = idlist.at(1).toUShort(&ok);

        if (ok && 0 < test)
            this->suffix = test;
        else
            this->suffix = defaultSuffix;
    }
    else {
        this->prefix += id;
        this->suffix = defaultSuffix;
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const QString& id)
    : QVariant(id), suffix(0)
{
    QStringList idlist = id.split(':',QString::SkipEmptyParts);
    if (2 == idlist.length()){

        this->prefix += idlist.at(0);

        bool ok;
        ushort test = idlist.at(1).toUShort(&ok);

        if (ok && 0 < test){

            this->suffix = test;
        }
    }
    else {
        this->prefix += id;
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const QByteArray& id)
    : QVariant(id), suffix(0)
{
    QString idstring(id);
    QStringList idlist = idstring.split(':',QString::SkipEmptyParts);
    if (2 == idlist.length()){

        this->prefix += idlist.at(0);

        bool ok;
        ushort test = idlist.at(1).toUShort(&ok);

        if (ok && 0 < test){

            this->suffix = test;
        }
    }
    else {
        this->prefix += id;
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const SystemDeviceIdentifier& deviceIdentifier)
    : QVariant(deviceIdentifier), suffix(0)
{
    this->prefix += deviceIdentifier.prefix;
    this->suffix = deviceIdentifier.suffix;
}
SystemDeviceIdentifier::~SystemDeviceIdentifier(){
}
bool SystemDeviceIdentifier::isSpecial() const {
    return (0 == this->prefix.length() && 0 == this->suffix);
}
bool SystemDeviceIdentifier::isValid() const {
    return (0 < this->prefix.length());
}
bool SystemDeviceIdentifier::isNotValid() const {
    return (0 == this->prefix.length());
}
bool SystemDeviceIdentifier::operator==(const SystemDeviceIdentifier* that) const {
    return (that && that->prefix == this->prefix && that->suffix == this->suffix);
}
bool SystemDeviceIdentifier::operator==(const SystemDeviceIdentifier& that) const {
    return (that.prefix == this->prefix && that.suffix == this->suffix);
}
bool SystemDeviceIdentifier::operator!=(const SystemDeviceIdentifier* that) const {
    return ((!that) || that->prefix != this->prefix || that->suffix != this->suffix);
}
bool SystemDeviceIdentifier::operator!=(const SystemDeviceIdentifier& that) const {
    return (that.prefix != this->prefix || that.suffix != this->suffix);
}
bool SystemDeviceIdentifier::operator<(const SystemDeviceIdentifier* that) const {
    return (that && this->prefix < that->prefix && this->suffix < that->suffix);
}
bool SystemDeviceIdentifier::operator<(const SystemDeviceIdentifier& that) const {
    return (this->prefix < that.prefix && this->suffix < that.suffix);
}
bool SystemDeviceIdentifier::operator<=(const SystemDeviceIdentifier* that) const {
    return (that && this->prefix <= that->prefix && this->suffix <= that->suffix);
}
bool SystemDeviceIdentifier::operator<=(const SystemDeviceIdentifier& that) const {
    return (this->prefix <= that.prefix && this->suffix <= that.suffix);
}
bool SystemDeviceIdentifier::operator>(const SystemDeviceIdentifier* that) const {
    return (that && this->prefix > that->prefix && this->suffix > that->suffix);
}
bool SystemDeviceIdentifier::operator>(const SystemDeviceIdentifier& that) const {
    return (this->prefix > that.prefix && this->suffix > that.suffix);
}
bool SystemDeviceIdentifier::operator>=(const SystemDeviceIdentifier* that) const {
    return (that && this->prefix >= that->prefix && this->suffix >= that->suffix);
}
bool SystemDeviceIdentifier::operator>=(const SystemDeviceIdentifier& that) const {
    return (this->prefix >= that.prefix && this->suffix >= that.suffix);
}
const QString& SystemDeviceIdentifier::getPrefix() const {
    return this->prefix;
}
quint16 SystemDeviceIdentifier::getSuffix() const {
    return this->suffix;
}
QString SystemDeviceIdentifier::toString() const {
    QString re("%1:%2");
    re.arg(prefix).arg(suffix);
    return re;
}
QStringList SystemDeviceIdentifier::toStringList() const {
    return toString().split(':',QString::SkipEmptyParts);
}
uint qHash ( const SystemDeviceIdentifier& s){
    if (s.isValid()){

        return (qHash(s.getPrefix()) ^ qHash(s.getSuffix()));
    }
    else {
        return 0;
    }
}
