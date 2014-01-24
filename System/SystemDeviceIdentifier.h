/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemDeviceIdentifier_H
#define _SYSTEM_SystemDeviceIdentifier_H

#include <QByteArray>
#include <QHash>
#include <QSharedData>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVariant>

/*!
 * Parse a device identifier string into a prefix and optional an
 * suffix -- separated by a single colon (':').  A default suffix
 * value is required by the constructor.
 */
class SystemDeviceIdentifier {
    /*!
     */
    struct SystemDeviceIdentifierInternal : public QSharedData {
        QString prefix;
        quint16 suffix;
        QString external;
        QString file;

        SystemDeviceIdentifierInternal()
            : QSharedData()
        {
        }
        SystemDeviceIdentifierInternal(const QString& p, const quint16 s)
            : QSharedData(), prefix(p), suffix(s), external(), file()
        {
            if (!p.isEmpty()){

                QString fp(p);
                fp.replace(".","_");

                if (0 != s){

                    external += "%1:%2";
                    file += "%1_%2";

                    external.arg(p).arg(s);

                    file.arg(fp).arg(s);
                }
                else {

                    external += p;

                    file += fp;
                }
            }
        }
        SystemDeviceIdentifierInternal(const SystemDeviceIdentifierInternal& copy)
            : QSharedData(copy), prefix(copy.prefix), suffix(copy.suffix), external(copy.external), file(copy.file)
        {
        }
    };

    static const int InitMetaTypeId;

    static QHash<QString,SystemDeviceIdentifier*> Heap;
    static QString Cat(const QString&, const quint16);

    QSharedDataPointer<SystemDeviceIdentifierInternal> internal;

 public:
    static const SystemDeviceIdentifier BroadcastIdentifier;
    /*!
     */
    static const SystemDeviceIdentifier& intern(const QString&);
    static const SystemDeviceIdentifier& intern(const QString&, const quint16);
    static const SystemDeviceIdentifier& intern(const QByteArray&);
    static const SystemDeviceIdentifier& intern();
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
    void swap(SystemDeviceIdentifier&);
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
