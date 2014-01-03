/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QList>

#include "TMTCNameValue.h"

TMTCNameValue::TMTCNameValue(const TMTCName& name)
    : QByteArray(), name(name)
{
    append(name.toByteArray());
    append('?');
}
TMTCNameValue::TMTCNameValue(const TMTCName& name, const QVariant& value)
    : QByteArray(), name(name), value(value)
{
    append(name.toByteArray());
    append('=');
    append(value.toByteArray());
}
/*!
 * Parse {N=V}, {N?} and {N} as {N,V} and {N}.
 */
TMTCNameValue::TMTCNameValue(const QByteArray& nvr)
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
TMTCNameValue::TMTCNameValue(const TMTCNameValue* nv){
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
TMTCNameValue::~TMTCNameValue(){
}
bool TMTCNameValue::isSpecial() const {
    return name.isSpecial();
}
bool TMTCNameValue::hasName() const {
    return name.isValid();
}
bool TMTCNameValue::hasNotName() const {
    return name.isNull();
}
bool TMTCNameValue::hasValue() const {
    return value.isValid();
}
bool TMTCNameValue::hasNotValue() const {
    return value.isNull();
}
const TMTCName& TMTCNameValue::getName() const {
    return name;
}
const QVariant& TMTCNameValue::getValue() const {
    return value;
}
bool TMTCNameValue::valueCanConvert(QVariant::Type type){

    return this->value.canConvert(type);
}
bool TMTCNameValue::valueConvert(QVariant::Type type){

    return this->value.convert(type);
}
