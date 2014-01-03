/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef TMTC_NAME_H
#define TMTC_NAME_H

#include <QByteArray>
#include <QHash>
#include <QMetaType>
#include <QString>
#include <QVariant>

/*!
 * 
 */
class TMTCName : public QVariant {

    static int InitMetaTypeId;

    mutable QString prefix;
    mutable QString suffix;

    mutable volatile bool parsed;

    const TMTCName& parse() const;

 public:
    /*!
     * An invalid name
     */
    TMTCName();
    /*!
     * Parse name from argument ASCII
     */
    TMTCName(QByteArray&);
    /*!
     * Parse name from argument ASCII
     */
    TMTCName(QString&);
    /*!
     * Parse name from argument ASCII
     */
    TMTCName(const char*);
    /*!
     * Copy name
     */
    TMTCName(const TMTCName&);
    /*!
     */
    ~TMTCName();
    /*!
     * This name supports special desktop semantics, user expressions
     * in this name may not be forwarded to the instrument device.
     */
    bool isSpecial() const;
    /*!
     */
    bool hasPrefix() const;
    /*!
     */
    bool hasNotPrefix() const;
    /*!
     */
    bool hasSuffix() const;
    /*!
     */
    bool hasNotSuffix() const;
    /*!
     */
    const QString& getPrefix() const;
    /*!
     */
    const QString& getSuffix() const;
    /*!
     */
    bool operator==(const TMTCName*) const;
    bool operator==(const TMTCName&) const;
    /*!
     */
    bool operator!=(const TMTCName*) const;
    bool operator!=(const TMTCName&) const;
    /*!
     */
    bool operator<(const TMTCName* that) const;
    bool operator<(const TMTCName& that) const;
    /*!
     */
    bool operator<=(const TMTCName* that) const;
    bool operator<=(const TMTCName& that) const;
    /*!
     */
    bool operator>(const TMTCName* that) const;
    bool operator>(const TMTCName& that) const;
    /*!
     */
    bool operator>=(const TMTCName* that) const;
    bool operator>=(const TMTCName& that) const;
};

uint qHash ( const TMTCName&);

#endif
