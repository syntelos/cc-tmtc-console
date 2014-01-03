/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _TMTC_NAMEVALUE_H
#define _TMTC_NAMEVALUE_H

#include <QByteArray>
#include <QObject>
#include <QVariant>

#include "TMTCName.h"

/*!
 * Component of TMTCMessage
 */
class TMTCNameValue : public QByteArray {

    TMTCName name;
    QVariant value;

 public:
    /*!
     */
    TMTCNameValue(const TMTCName& name);
    /*!
     */
    TMTCNameValue(const TMTCName& name, const QVariant& value);
    /*!
     * Parse "N=V", "N?" and "N" as {N,V} and {N}.
     */
    TMTCNameValue(const QByteArray& nv);
    /*!
     */
    TMTCNameValue(const TMTCNameValue* nv);
    /*!
     */
    ~TMTCNameValue();
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
    const TMTCName& getName() const;
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
