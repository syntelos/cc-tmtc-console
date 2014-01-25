/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QList>

#include "SystemNameValue.h"

SystemNameValue::SystemNameValue(const SystemName& name)
    : QByteArray(), name(name)
{
    append(name.toByteArray());
    append('?');
}
SystemNameValue::SystemNameValue(const SystemName& name, const QVariant& value)
    : QByteArray(), name(name), value(value)
{
    append(name.toByteArray());

    if (value.isNull()){

        append('?');
    }
    else {
        append('=');
        append(value.toByteArray());
    }
}
/*!
 * Parse {N=V}, {N?} and {N} as {N,V} and {N}.
 */
SystemNameValue::SystemNameValue(const QByteArray& nvr)
{
    QList<QByteArray> list = nvr.split('=');

    switch (list.count()){
    case 1:
        {
            QByteArray n0 = list.at(0);
            QByteArray nt = n0.trimmed();
            if (nt.endsWith('?')){
                QByteArray trunc = nt.left(nt.size()-1);
                QByteArray trimmed = trunc.trimmed();
                name.setValue(trimmed);
            }
            else {
                name.setValue(nt);
            }
            append(nt);
            append('?');
        }
        break;
    case 2:
        {
            QByteArray n0 = list.at(0);
            QByteArray nt = n0.trimmed();
            name.setValue(nt);

            QByteArray v1 = list.at(1);
            QByteArray vt = v1.trimmed();

            if (0 < vt.length()){

                value.setValue(vt);

                append(nt);
                append('=');
                append(vt);
            }
            else {
                append('?');
            }
        }
        break;
    default:
        break;
    }
}
SystemNameValue::SystemNameValue(const SystemNameValue* nv){
    if (nv){
        if (nv->hasName()){

            QByteArray n = nv->name.toByteArray();

            this->name.setValue(n);

            this->append(n);

            if (nv->hasValue()){

                QByteArray v = nv->value.toByteArray();

                this->value.setValue(v);

                this->append('=');
                this->append(v);
            }
            else {
                this->append('?');
            }
        }
    }
}
/*!
 */
SystemNameValue::~SystemNameValue(){
}
bool SystemNameValue::isSpecial() const {
    return name.isSpecial();
}
bool SystemNameValue::hasName() const {
    return name.isValid();
}
bool SystemNameValue::hasNotName() const {
    return name.isNull();
}
bool SystemNameValue::hasValue() const {
    return value.isValid();
}
bool SystemNameValue::hasNotValue() const {
    return value.isNull();
}
const SystemName& SystemNameValue::getName() const {
    return name;
}
const QVariant& SystemNameValue::getValue() const {
    return value;
}
bool SystemNameValue::setValue(const QVariant& v){
    value.setValue(v);
    QByteArray::clear();

    QByteArray::append(name.toByteArray());
    if (value.isNull()){

        QByteArray::append('?');
    }
    else {
        QByteArray::append('=');
        QByteArray::append(value.toByteArray());
    }
    return true;
}
bool SystemNameValue::valueCanConvert(QVariant::Type type){

    return this->value.canConvert(type);
}
bool SystemNameValue::valueConvert(QVariant::Type type){

    return this->value.convert(type);
}
bool SystemNameValue::operator==(const SystemName* that) const {

    return (name == that);
}
bool SystemNameValue::operator==(const SystemName& that) const {

    return (name == that);
}
bool SystemNameValue::operator!=(const SystemName* that) const {

    return (name != that);
}
bool SystemNameValue::operator!=(const SystemName& that) const {

    return (name != that);
}
bool SystemNameValue::operator<(const SystemName* that) const {

    return (name < that);
}
bool SystemNameValue::operator<(const SystemName& that) const {

    return (name < that);
}
bool SystemNameValue::operator<=(const SystemName* that) const {

    return (name <= that);
}
bool SystemNameValue::operator<=(const SystemName& that) const {

    return (name <= that);
}
bool SystemNameValue::operator>(const SystemName* that) const {

    return (name > that);
}
bool SystemNameValue::operator>(const SystemName& that) const {

    return (name > that);
}
bool SystemNameValue::operator>=(const SystemName* that) const {

    return (name >= that);
}
bool SystemNameValue::operator>=(const SystemName& that) const {

    return (name >= that);
}

