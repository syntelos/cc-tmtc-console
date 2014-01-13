/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QList>
#include <QSettings>

#include "MultiplexIndex.h"
#include "MultiplexRecord.h"

MultiplexIndex::MultiplexIndex(const SystemDeviceIdentifier& id)
    : id(id), prefix(id.toString()), table(),
      object_size(MX::RecordBase),
      ofs_first(0), ofs_last(-1),
      count_temporal(1), count_spatial(9), count_user(3610)
{
    read();
}
MultiplexIndex::MultiplexIndex(const MultiplexIndex& copy)
    : id(copy.id), table(copy.table)
{
}
MultiplexIndex::~MultiplexIndex()
{
}
qptrdiff MultiplexIndex::getObjectSize() const {

    return object_size;
}
void MultiplexIndex::setObjectSize(qptrdiff size){

    if (0 < size){
        object_size = size;
    }
}
bool MultiplexIndex::maxObjectSize(qptrdiff size){

    if (size > object_size){
        object_size = size;

        return true;
    }
    else
        return false;
}
qptrdiff MultiplexIndex::getFirst() const {

    return ofs_first;
}
void MultiplexIndex::readFirst(const QVariant& ofs){

    if (ofs.isValid()){

        ofs_first = (qptrdiff)(ofs.toLongLong());
    }
}
void MultiplexIndex::setFirst(qptrdiff ofs){

    if (0 <= ofs){

        ofs_first = ofs;
    }
}
qptrdiff MultiplexIndex::getLast() const {

    return ofs_last;
}
void MultiplexIndex::readLast(const QVariant& ofs){

    if (ofs.isValid()){

        ofs_last = (qptrdiff)(ofs.toLongLong());
    }
}
void MultiplexIndex::setLast(qptrdiff ofs){

    if (0 <= ofs){

        ofs_last = ofs;
    }
}
quint32 MultiplexIndex::getCountTemporal() const {

    return count_temporal;
}
void MultiplexIndex::setCountTemporal(quint32 count){

    if (0 != count){

        count_temporal = count;
    }
}
quint32 MultiplexIndex::getCountSpatial() const {

    return count_spatial;
}
void MultiplexIndex::setCountSpatial(quint32 count){

    if (0 != count){

        count_spatial = count;
    }
}
quint32 MultiplexIndex::getCountUser() const {

    return count_user;
}
void MultiplexIndex::setCountUser(quint32 count){

    if (0 != count){
        count_user = count;
    }
}
quint32 MultiplexIndex::getRecordCount() const {

    return (count_temporal + count_spatial + count_user);
}
bool MultiplexIndex::read(){
    QSettings settings;
    /*
     * Object size
     */
    QString object_size_key(prefix);
    {
        object_size_key += '/';
        object_size_key += "object_size";
    }
    setObjectSize( (qptrdiff)settings.value(object_size_key).toULongLong());
    /*
     * First
     */
    QString first_key(prefix);
    {
        first_key += '/';
        first_key += "first";
    }
    readFirst( settings.value(first_key));
    /*
     * Last
     */
    QString last_key(prefix);
    {
        last_key += '/';
        last_key += "last";
    }
    readLast( settings.value(last_key));
    /*
     * Count_Temporal
     */
    QString count_temporal_key(prefix);
    {
        count_temporal_key += '/';
        count_temporal_key += "count_temporal";
    }
    setCountTemporal( (qptrdiff)settings.value(count_temporal_key).toULongLong());
    /*
     * Count_Spatial
     */
    QString count_spatial_key(prefix);
    {
        count_spatial_key += '/';
        count_spatial_key += "count_spatial";
    }
    setCountSpatial( (qptrdiff)settings.value(count_spatial_key).toULongLong());
    /*
     * Count_User
     */
    QString count_user_key(prefix);
    {
        count_user_key += '/';
        count_user_key += "count_user";
    }
    setCountUser( (qptrdiff)settings.value(count_user_key).toULongLong());
    /*
     * Index table
     */
    int count = settings.beginReadArray(prefix);
    if (0 < count){

        table.clear();

        int cc;
        for (cc = 0; cc < count; cc++){
            settings.setArrayIndex(cc);

            TMTCName name = settings.value("name").value<TMTCName>();

            int index = settings.value("index").toInt();

            table[name] = index;
        }
    }
    settings.endArray();

    return (0 < count);
}
bool MultiplexIndex::write() const {
    QSettings settings;
    /*
     * Object size
     */
    QString object_size_key(prefix);
    {
        object_size_key += '/';
        object_size_key += "object_size";
    }
    settings.setValue(object_size_key,object_size);
    /*
     * First
     */
    QString first_key(prefix);
    {
        first_key += '/';
        first_key += "first";
    }
    settings.setValue(first_key,ofs_first);
    /*
     * Last
     */
    if (-1 != ofs_last){
        QString last_key(prefix);
        {
            last_key += '/';
            last_key += "last";
        }
        settings.setValue(last_key,ofs_last);
    }
    /*
     * Count_Temporal
     */
    QString count_temporal_key(prefix);
    {
        count_temporal_key += '/';
        count_temporal_key += "count_temporal";
    }
    settings.setValue(count_temporal_key,count_temporal);
    /*
     * Count_Spatial
     */
    QString count_spatial_key(prefix);
    {
        count_spatial_key += '/';
        count_spatial_key += "count_spatial";
    }
    settings.setValue(count_spatial_key,count_spatial);
    /*
     * Count_User
     */
    QString count_user_key(prefix);
    {
        count_user_key += '/';
        count_user_key += "count_user";
    }
    settings.setValue(count_user_key,count_user);
    /*
     * Index table
     */
    int count = table.size();
    if (0 < count){
        settings.beginWriteArray(prefix,count);

        QList<TMTCName> keys = table.keys();

        int cc;
        for (cc = 0; cc < count; cc++){
            settings.setArrayIndex(cc);

            TMTCName name = keys.at(cc);

            int index = table[name];

            settings.setValue("name",name);
            settings.setValue("index",index);
        }
        settings.endArray();
        return true;
    }
    else
        return false;
}
int MultiplexIndex::count() const {
    return table.size();
}
bool MultiplexIndex::contains(const TMTCName & n) const {
    return table.contains(n);
}
int& MultiplexIndex::operator[](const TMTCName & n){
    return table[n];
}
int MultiplexIndex::operator[](const TMTCName & n) const {
    return table[n];
}



