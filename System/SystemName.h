/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef System_NAME_H
#define System_NAME_H

#include <QByteArray>
#include <QHash>
#include <QMetaType>
#include <QString>
#include <QVariant>

/*!
 * 
 */
class SystemName : public QVariant {

    static const int InitMetaTypeId;

    mutable QString prefix;
    mutable QString suffix;

    mutable volatile bool parsed;

    const SystemName& parse() const;

 public:
    /*!
     * An invalid name
     */
    SystemName();
    /*!
     * Parse name from argument ASCII
     */
    SystemName(QByteArray&);
    /*!
     * Parse name from argument ASCII
     */
    SystemName(QVariant&);
    /*!
     * Parse name from argument ASCII
     */
    SystemName(QString&);
    /*!
     * Parse name from argument ASCII
     */
    SystemName(const char*);
    /*!
     * Copy name
     */
    SystemName(const SystemName&);
    /*!
     */
    ~SystemName();
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
    bool operator==(const SystemName*) const;
    bool operator==(const SystemName&) const;
    /*!
     */
    bool operator!=(const SystemName*) const;
    bool operator!=(const SystemName&) const;
    /*!
     */
    bool operator<(const SystemName* that) const;
    bool operator<(const SystemName& that) const;
    /*!
     */
    bool operator<=(const SystemName* that) const;
    bool operator<=(const SystemName& that) const;
    /*!
     */
    bool operator>(const SystemName* that) const;
    bool operator>(const SystemName& that) const;
    /*!
     */
    bool operator>=(const SystemName* that) const;
    bool operator>=(const SystemName& that) const;
};

uint qHash ( const SystemName&);

Q_DECLARE_METATYPE(SystemName);
#endif
