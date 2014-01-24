/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _System_NAMEVALUE_H
#define _System_NAMEVALUE_H

#include <QByteArray>
#include <QObject>
#include <QVariant>

#include "SystemName.h"

/*!
 * Component of SystemMessage
 */
class SystemNameValue : public QByteArray {

    SystemName name;
    QVariant value;

 public:
    /*!
     */
    SystemNameValue(const SystemName& name);
    /*!
     */
    SystemNameValue(const SystemName& name, const QVariant& value);
    /*!
     * Parse "N=V", "N?" and "N" as {N,V} and {N}.
     */
    SystemNameValue(const QByteArray& nv);
    /*!
     */
    SystemNameValue(const SystemNameValue* nv);
    /*!
     */
    ~SystemNameValue();
    /*!
     */
    bool isSpecial() const;
    /*!
     */
    bool hasName() const;
    /*!
     */
    bool hasNotName() const;
    /*!
     */
    bool hasValue() const;
    /*!
     */
    bool hasNotValue() const;
    /*!
     */
    const SystemName& getName() const;
    /*!
     * When the returned value is not null, the const requirement may
     * be lowered.
     * 
     * When the value exists, it may have a string, numeric or other
     * type.  The constructor will create a \class QVariant from the
     * input \class QByteArray, and this class exposes the QVariant
     * conversion methods which are likewise available via the
     * returned object reference.
     */
    const QVariant& getValue() const;

    bool valueCanConvert(QVariant::Type type);

    bool valueConvert(QVariant::Type type);

};

#endif
