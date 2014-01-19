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
    static const int InitMetaTypeId;

    mutable QString prefix;
    mutable QString suffix;
    mutable bool parsed;

    void parse() const;

 public:
    SystemScriptSymbol();
    SystemScriptSymbol(const SystemScriptSymbol&);
    SystemScriptSymbol(const char*);
    SystemScriptSymbol(const char*, int);
    SystemScriptSymbol(const QByteArray&);
    SystemScriptSymbol(const QString&);
    SystemScriptSymbol(const QString& className, const QString& identifier);
    ~SystemScriptSymbol();
    /*!
     */
    void clear();
    /*!
     */
    void set(const QString& string);
    /*!
     */
    void set(const QByteArray& string);
    /*!
     */
    void set(const char *string);
    /*!
     */
    void set(const char *string, int len);
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

    const char* slot(const QMetaObject* type) const;

    QString toString() const;
};

Q_DECLARE_METATYPE(SystemScriptSymbol);

#endif
