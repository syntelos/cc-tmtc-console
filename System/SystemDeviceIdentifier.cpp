/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QMetaType>
#include <QStringList>

#include "SystemDeviceIdentifier.h"


const int SystemDeviceIdentifier::InitMetaTypeId = qRegisterMetaType<SystemDeviceIdentifier>("SystemDeviceIdentifier");

QHash<QString,SystemDeviceIdentifier*> SystemDeviceIdentifier::Heap;

const SystemDeviceIdentifier SystemDeviceIdentifier::BroadcastIdentifier;

QString SystemDeviceIdentifier::Cat(const QString& id, const quint16 defaultSuffix){
    if (0 < id.indexOf(':'))
        return id;
    else {
        QString re("%1:%2");
        re.arg(id).arg(defaultSuffix);
        return re;
    }
}
const SystemDeviceIdentifier& SystemDeviceIdentifier::intern(const QString& id){
    /*
     * Accumulate the knowledge of unique non zero suffix values with
     * the following map structures.
     * 
     * An argument having no suffix or a zero valued suffix is handled
     * as a prefix query.
     * 
     * For one "prefix:suffix" pair:
     *
     *   map(prefix) -> (sid)
     *   map(prefix:suffix) -> (sid)
     * 
     * For multiple pairs sharing a prefix with unique non zero suffix
     * values:
     *
     *   map(prefix) -> {sid0,...,sidN}
     *   map(prefix:suffix) -> (sid)
     * 
     * A prefix query into a multiple mapped prefix returns any member
     * of the mapped set.
     */
    SystemDeviceIdentifier* test = new SystemDeviceIdentifier(id);
    quint16 testSuffix = test->getSuffix();

    const QString& prefix = test->getPrefix();
    QString key = test->toString();

    if (Heap.contains(prefix)){
        SystemDeviceIdentifier* mapped = Heap.value(prefix);
        quint16 mappedSuffix = mapped->getSuffix();

        if (mappedSuffix == testSuffix){
            /*
             * Query
             */
            delete test;

            return *mapped;
        }
        else if (0 == mappedSuffix){
            /*
             * Refine SID
             */
            mapped->swap(*test);
            /*
             * Remap SID
             */
            Heap.insert(key,mapped);

            delete test;

            return *mapped;
        }
        else if (0 == testSuffix){
            /*
             * Query
             */
            delete test;

            return *mapped;
        }
        else {
            /*
             * map(prefix) -> {sid0,...,sidN}
             */
            Heap.insertMulti(prefix,test);
            /*
             * map(prefix:suffix) -> (sid)
             */
            Heap.insert(key,test);

            return *test;
        }
    }
    else {
        /*
         * map(prefix) -> (sid)
         */
        Heap.insert(prefix,test);

        if (0 != testSuffix){
            /*
             * map(prefix:suffix) -> (sid)
             */
            Heap.insert(key,test);
        }
        return *test;
    }
}
const SystemDeviceIdentifier& SystemDeviceIdentifier::intern(const QString& id, 
                                                             const quint16 defaultSuffix)
{
    return SystemDeviceIdentifier::intern(Cat(id,defaultSuffix));
}
const SystemDeviceIdentifier& SystemDeviceIdentifier::intern(const QByteArray& id)
{
    return SystemDeviceIdentifier::intern(QString(id));
}
const SystemDeviceIdentifier& SystemDeviceIdentifier::intern()
{
    return BroadcastIdentifier;
}
SystemDeviceIdentifier::SystemDeviceIdentifier()
    : internal()
{
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const QString& id, const quint16 defaultSuffix)
    : internal()
{
    QStringList idlist = id.split(':',QString::SkipEmptyParts);
    if (2 == idlist.length()){

        QString p = idlist.at(0);

        bool ok;
        ushort s = idlist.at(1).toUShort(&ok);

        if (ok && 0 < s){
            internal = new SystemDeviceIdentifierInternal(p,s);
        }
        else {
            internal = new SystemDeviceIdentifierInternal(p,defaultSuffix);
        }
    }
    else {
        internal = new SystemDeviceIdentifierInternal(id,defaultSuffix);
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const QString& id)
    : internal()
{
    QStringList idlist = id.split(':',QString::SkipEmptyParts);
    if (2 == idlist.length()){

        QString p = idlist.at(0);

        bool ok;
        ushort s = idlist.at(1).toUShort(&ok);

        if (ok && 0 < s){

            internal = new SystemDeviceIdentifierInternal(p,s);
        }
        else {

            internal = new SystemDeviceIdentifierInternal(p,0);
        }
    }
    else {
        internal = new SystemDeviceIdentifierInternal(id,0);
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const QByteArray& id)
    : internal()
{
    QString idstring(id);
    QStringList idlist = idstring.split(':',QString::SkipEmptyParts);
    if (2 == idlist.length()){

        QString p = idlist.at(0);

        bool ok;
        ushort s = idlist.at(1).toUShort(&ok);

        if (ok && 0 < s){

            internal = new SystemDeviceIdentifierInternal(p,s);
        }
        else {

            internal = new SystemDeviceIdentifierInternal(p,0);
        }
    }
    else {
        internal = new SystemDeviceIdentifierInternal(id,0);
    }
}
SystemDeviceIdentifier::SystemDeviceIdentifier(const SystemDeviceIdentifier& deviceIdentifier)
    : internal(deviceIdentifier.internal)
{
}
SystemDeviceIdentifier::~SystemDeviceIdentifier(){
    /*
     * delete internal (not)
     */
}
bool SystemDeviceIdentifier::isSpecial() const {
    return (0 == internal->prefix.length() && 0 == internal->suffix);
}
bool SystemDeviceIdentifier::isValid() const {
    return (0 < internal->prefix.length());
}
bool SystemDeviceIdentifier::isNotValid() const {
    return (0 == internal->prefix.length());
}
void SystemDeviceIdentifier::swap(SystemDeviceIdentifier& that){

    internal.swap(that.internal);
}
bool SystemDeviceIdentifier::operator==(const SystemDeviceIdentifier* that) const {
    return (that && that->internal->prefix == internal->prefix && that->internal->suffix == internal->suffix);
}
bool SystemDeviceIdentifier::operator==(const SystemDeviceIdentifier& that) const {
    return (that.internal->prefix == internal->prefix && that.internal->suffix == internal->suffix);
}
bool SystemDeviceIdentifier::operator!=(const SystemDeviceIdentifier* that) const {
    return ((!that) || that->internal->prefix != internal->prefix || that->internal->suffix != internal->suffix);
}
bool SystemDeviceIdentifier::operator!=(const SystemDeviceIdentifier& that) const {
    return (that.internal->prefix != internal->prefix || that.internal->suffix != internal->suffix);
}
bool SystemDeviceIdentifier::operator<(const SystemDeviceIdentifier* that) const {
    return (that && internal->prefix < that->internal->prefix && internal->suffix < that->internal->suffix);
}
bool SystemDeviceIdentifier::operator<(const SystemDeviceIdentifier& that) const {
    return (internal->prefix < that.internal->prefix && internal->suffix < that.internal->suffix);
}
bool SystemDeviceIdentifier::operator<=(const SystemDeviceIdentifier* that) const {
    return (that && internal->prefix <= that->internal->prefix && internal->suffix <= that->internal->suffix);
}
bool SystemDeviceIdentifier::operator<=(const SystemDeviceIdentifier& that) const {
    return (internal->prefix <= that.internal->prefix && internal->suffix <= that.internal->suffix);
}
bool SystemDeviceIdentifier::operator>(const SystemDeviceIdentifier* that) const {
    return (that && internal->prefix > that->internal->prefix && internal->suffix > that->internal->suffix);
}
bool SystemDeviceIdentifier::operator>(const SystemDeviceIdentifier& that) const {
    return (internal->prefix > that.internal->prefix && internal->suffix > that.internal->suffix);
}
bool SystemDeviceIdentifier::operator>=(const SystemDeviceIdentifier* that) const {
    return (that && internal->prefix >= that->internal->prefix && internal->suffix >= that->internal->suffix);
}
bool SystemDeviceIdentifier::operator>=(const SystemDeviceIdentifier& that) const {
    return (internal->prefix >= that.internal->prefix && internal->suffix >= that.internal->suffix);
}
const QString& SystemDeviceIdentifier::getPrefix() const {
    return internal->prefix;
}
quint16 SystemDeviceIdentifier::getSuffix() const {
    return internal->suffix;
}
QString SystemDeviceIdentifier::toString() const {
    return internal->external;
}
QString SystemDeviceIdentifier::toString(const QString& fext) const {

    if (isSpecial()){

        return fext;
    }
    else if (fext.isEmpty()){

        return internal->file;
    }
    else {
        return QString("%1.%2").arg(internal->file,fext);
    }
}
QStringList SystemDeviceIdentifier::toStringList() const {
    return toString().split(':',QString::SkipEmptyParts);
}
uint qHash ( const SystemDeviceIdentifier& s){
    if (s.isValid()){

        return (qHash(s.getPrefix()) ^ qHash(s.getSuffix()));
    }
    else {
        return 0;
    }
}
