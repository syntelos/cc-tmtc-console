/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemScriptSymbol_H
#define _SYSTEM_SystemScriptSymbol_H

#include <QByteArray>
#include <QMetaObject>
#include <QString>
#include <QVariant>

/*!
 * A script signal or function symbolic reference.
 * 
 * A native symbolic reference employs class name prefix and method
 * name suffix (using C++ scope resolution syntax).  
 * 
 * A script reference only needs a function name identifier -- a
 * symbolic suffix.
 */
class SystemScriptSymbol
    : public QByteArray
{
    mutable QString prefix;
    mutable QString suffix;
    mutable bool parsed;

    void parse() const;

 public:
    SystemScriptSymbol();
    SystemScriptSymbol(const char* string);
    SystemScriptSymbol(const QByteArray& string);
    SystemScriptSymbol(const QString& string);
    SystemScriptSymbol(const QString& className, const QString& identifier);
    ~SystemScriptSymbol();
    /*!
     * Indempotent meta type registration is called by constructors
     * and may be called before any other use of this type for stream
     * input.
     */
    static void Init();
    /*!
     * Equivalent to "is empty" from \class QByteArray
     */
    bool isInert() const;
    /*!
     * Equivalent to "is not empty" from \class QByteArray
     */
    bool isNotInert() const;
    /*!
     * Prefix of symbol may be empty.  This is a class name for a
     * native symbolic reference, and could be a file name for a
     * script symbolic reference.
     */
    const QString& getPrefix() const;
    /*!
     */
    const char* getPrefixSymbolic() const;
    /*!
     * Suffix of symbol should not be empty.  This is a method name
     * for a native symbolic reference, and a function name for a
     * script symbolic reference.
     */
    const QString& getSuffix() const;
    /*!
     */
    const char* getSuffixSymbolic() const;
    /*!
     */
    bool operator==(const QObject* type) const;

    bool operator==(const QMetaObject* type) const;

    bool operator==(const SystemScriptSymbol* type) const;

    bool operator==(const SystemScriptSymbol& type) const;

    bool operator==(const char* type) const;

    bool operator==(const QString& type) const;
    /*!
     */
    bool operator!=(const QObject* type) const;

    bool operator!=(const QMetaObject* type) const;

    bool operator!=(const SystemScriptSymbol* type) const;

    bool operator!=(const SystemScriptSymbol& type) const;

    bool operator!=(const char* type) const;

    bool operator!=(const QString& type) const;
    /*!
     * Return a valid signal identifier for this symbolic reference.
     * This production concatenates the signal index number with the
     * method signature returned from \class QMetaMethod.
     */
    const char* signal(const QMetaObject* type) const;

    QString toString() const;
};

Q_DECLARE_METATYPE(SystemScriptSymbol);

#endif
