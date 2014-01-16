/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QStringList>

#include "TMTCName.h"


int TMTCName::InitMetaTypeId = qRegisterMetaType<TMTCName>("TMTCName");

TMTCName::TMTCName()
    : QVariant(), parsed(true)
{
}
TMTCName::TMTCName(QByteArray& b)
    : QVariant(b), parsed(false)
{
}
TMTCName::TMTCName(QVariant& v)
    : QVariant(v), parsed(false)
{
}
TMTCName::TMTCName(QString& s)
    : QVariant(s), parsed(false)
{
}
TMTCName::TMTCName(const char* s)
    : QVariant(s), parsed(false)
{
}
TMTCName::TMTCName(const TMTCName& n)
    : QVariant(n), parsed(n.parsed), prefix(n.prefix), suffix(n.suffix)
{
}
TMTCName::~TMTCName(){
}
const TMTCName& TMTCName::parse() const {
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
bool TMTCName::isSpecial() const {

    const QString& prefix = parse().prefix;
    /*
     * [TODO] Add Library
     */
    return (prefix == "M" || prefix == "m");
}
bool TMTCName::hasPrefix() const {
    return (!parse().prefix.isEmpty());
}
bool TMTCName::hasNotPrefix() const {
    return (parse().prefix.isEmpty());
}
bool TMTCName::hasSuffix() const {
    return (!parse().suffix.isEmpty());
}
bool TMTCName::hasNotSuffix() const {
    return (parse().suffix.isEmpty());
}
const QString& TMTCName::getPrefix() const {
    return parse().prefix;
}
const QString& TMTCName::getSuffix() const {
    return parse().suffix;
}
bool TMTCName::operator==(const TMTCName* that) const {
    return (that && that->getPrefix() == this->getPrefix() && that->getSuffix() == this->getSuffix());
}
bool TMTCName::operator==(const TMTCName& that) const {
    return (that.getPrefix() == this->getPrefix() && that.getSuffix() == this->getSuffix());
}
bool TMTCName::operator!=(const TMTCName* that) const {
    return ((!that) || that->getPrefix() != this->getPrefix() || that->getSuffix() != this->getSuffix());
}
bool TMTCName::operator!=(const TMTCName& that) const {
    return (that.getPrefix() != this->getPrefix() || that.getSuffix() != this->getSuffix());
}
bool TMTCName::operator<(const TMTCName* that) const {
    return (that && this->getPrefix() < that->getPrefix() && this->getSuffix() < that->getSuffix());
}
bool TMTCName::operator<(const TMTCName& that) const {
    return (this->getPrefix() < that.getPrefix() && this->getSuffix() < that.getSuffix());
}
bool TMTCName::operator<=(const TMTCName* that) const {
    return (that && this->getPrefix() <= that->getPrefix() && this->getSuffix() <= that->getSuffix());
}
bool TMTCName::operator<=(const TMTCName& that) const {
    return (this->getPrefix() <= that.getPrefix() && this->getSuffix() <= that.getSuffix());
}
bool TMTCName::operator>(const TMTCName* that) const {
    return (that && this->getPrefix() > that->getPrefix() && this->getSuffix() > that->getSuffix());
}
bool TMTCName::operator>(const TMTCName& that) const {
    return (this->getPrefix() > that.getPrefix() && this->getSuffix() > that.getSuffix());
}
bool TMTCName::operator>=(const TMTCName* that) const {
    return (that && this->getPrefix() >= that->getPrefix() && this->getSuffix() >= that->getSuffix());
}
bool TMTCName::operator>=(const TMTCName& that) const {
    return (this->getPrefix() >= that.getPrefix() && this->getSuffix() >= that.getSuffix());
}

uint qHash ( const TMTCName& n){

    if (n.hasSuffix()){

        return (qHash(n.getPrefix()) ^ qHash(n.getSuffix()));
    }
    else if (n.hasPrefix()){

        return qHash(n.getPrefix());
    }
    else
        return 0;
}
