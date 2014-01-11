/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDeviceIdentifier_H
#define _SYSTEM_SystemDeviceIdentifier_H

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QVariant>

/*!
 * Parse a device identifier string into a prefix and optional an
 * suffix -- separated by a single colon (':').  A default suffix
 * value is required by the constructor.
 */
class SystemDeviceIdentifier : public QVariant {

    static const int InitMetaTypeId;

    QString prefix;
    quint16 suffix;

 public:
    /*!
     * An invalid identifier may be employed for broadcast semantics
     * in some cases.
     */
    static const SystemDeviceIdentifier* BroadcastIdentifier;

    static QString Cat(const QString&, const quint16);
    /*!
     * Construct an invalid identifier
     */
    SystemDeviceIdentifier();
    /*!
     * Construct an identifier using a default suffix
     */
    SystemDeviceIdentifier(const QString&, const quint16);
    /*!
     * Construct an identifier
     */
    SystemDeviceIdentifier(const QString&);
    /*!
     * Construct an identifier
     */
    SystemDeviceIdentifier(const QByteArray&);
    /*!
     * Copy an identifier
     */
    SystemDeviceIdentifier(const SystemDeviceIdentifier&);
    /*!
     */
    ~SystemDeviceIdentifier();
    /*!
     * The specifically invalid has no prefix and no suffix, and may
     * be employed with broadcast semantics.  This test is necessary
     * to the correctness of the operator methods in this case.
     */
    bool isSpecial() const;
    /*!
     */
    bool isValid() const;
    /*!
     */
    bool isNotValid() const;
    /*!
     */
    const QString& getPrefix() const;
    /*!
     */
    quint16 getSuffix() const;
    /*!
     * Normal form
     */
    QString toString() const;
    /*!
     * Filename form
     */
    QString toString(const QString& fext) const ;

    QStringList toStringList() const;
    /*!
     */
    bool operator==(const SystemDeviceIdentifier*) const;
    bool operator==(const SystemDeviceIdentifier&) const;
    /*!
     */
    bool operator!=(const SystemDeviceIdentifier*) const;
    bool operator!=(const SystemDeviceIdentifier&) const;
    /*!
     */
    bool operator<(const SystemDeviceIdentifier* that) const;
    bool operator<(const SystemDeviceIdentifier& that) const;
    /*!
     */
    bool operator<=(const SystemDeviceIdentifier* that) const;
    bool operator<=(const SystemDeviceIdentifier& that) const;
    /*!
     */
    bool operator>(const SystemDeviceIdentifier* that) const;
    bool operator>(const SystemDeviceIdentifier& that) const;
    /*!
     */
    bool operator>=(const SystemDeviceIdentifier* that) const;
    bool operator>=(const SystemDeviceIdentifier& that) const;
};

uint qHash ( const SystemDeviceIdentifier&);

Q_DECLARE_METATYPE(SystemDeviceIdentifier);
#endif
