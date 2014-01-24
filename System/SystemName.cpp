/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QStringList>

#include "SystemName.h"


const int SystemName::InitMetaTypeId = qRegisterMetaType<SystemName>("SystemName");

SystemName::SystemName()
    : QVariant(), parsed(true)
{
}
SystemName::SystemName(QByteArray& b)
    : QVariant(b), parsed(false)
{
}
SystemName::SystemName(QVariant& v)
    : QVariant(v), parsed(false)
{
}
SystemName::SystemName(QString& s)
    : QVariant(s), parsed(false)
{
}
SystemName::SystemName(const char* s)
    : QVariant(s), parsed(false)
{
}
SystemName::SystemName(const SystemName& n)
    : QVariant(n), parsed(n.parsed), prefix(n.prefix), suffix(n.suffix)
{
}
SystemName::~SystemName(){
}
const SystemName& SystemName::parse() const {
    if (!this->parsed){
        this->parsed = true;
        const QString string = toString();

        const QStringList list = string.split('.',QString::SkipEmptyParts);
        const int count = list.length();
        if (2 <= count){
            this->prefix += list.at(0);
            this->suffix += list.at(1);
        }
        else if (1 == count){
            this->prefix += list.at(0);
        }
    }
    return *this;
}
bool SystemName::isSpecial() const {

    const QString& prefix = parse().prefix;
    /*
     * [TODO] Add Library
     */
    return (prefix == "M" || prefix == "m");
}
bool SystemName::hasPrefix() const {
    return (!parse().prefix.isEmpty());
}
bool SystemName::hasNotPrefix() const {
    return (parse().prefix.isEmpty());
}
bool SystemName::hasSuffix() const {
    return (!parse().suffix.isEmpty());
}
bool SystemName::hasNotSuffix() const {
    return (parse().suffix.isEmpty());
}
const QString& SystemName::getPrefix() const {
    return parse().prefix;
}
const QString& SystemName::getSuffix() const {
    return parse().suffix;
}
bool SystemName::operator==(const SystemName* that) const {
    return (that && that->getPrefix() == this->getPrefix() && that->getSuffix() == this->getSuffix());
}
bool SystemName::operator==(const SystemName& that) const {
    return (that.getPrefix() == this->getPrefix() && that.getSuffix() == this->getSuffix());
}
bool SystemName::operator!=(const SystemName* that) const {
    return ((!that) || that->getPrefix() != this->getPrefix() || that->getSuffix() != this->getSuffix());
}
bool SystemName::operator!=(const SystemName& that) const {
    return (that.getPrefix() != this->getPrefix() || that.getSuffix() != this->getSuffix());
}
bool SystemName::operator<(const SystemName* that) const {
    return (that && this->getPrefix() < that->getPrefix() && this->getSuffix() < that->getSuffix());
}
bool SystemName::operator<(const SystemName& that) const {
    return (this->getPrefix() < that.getPrefix() && this->getSuffix() < that.getSuffix());
}
bool SystemName::operator<=(const SystemName* that) const {
    return (that && this->getPrefix() <= that->getPrefix() && this->getSuffix() <= that->getSuffix());
}
bool SystemName::operator<=(const SystemName& that) const {
    return (this->getPrefix() <= that.getPrefix() && this->getSuffix() <= that.getSuffix());
}
bool SystemName::operator>(const SystemName* that) const {
    return (that && this->getPrefix() > that->getPrefix() && this->getSuffix() > that->getSuffix());
}
bool SystemName::operator>(const SystemName& that) const {
    return (this->getPrefix() > that.getPrefix() && this->getSuffix() > that.getSuffix());
}
bool SystemName::operator>=(const SystemName* that) const {
    return (that && this->getPrefix() >= that->getPrefix() && this->getSuffix() >= that->getSuffix());
}
bool SystemName::operator>=(const SystemName& that) const {
    return (this->getPrefix() >= that.getPrefix() && this->getSuffix() >= that.getSuffix());
}

uint qHash ( const SystemName& n){

    if (n.hasSuffix()){

        return (qHash(n.getPrefix()) ^ qHash(n.getSuffix()));
    }
    else if (n.hasPrefix()){

        return qHash(n.getPrefix());
    }
    else
        return 0;
}
