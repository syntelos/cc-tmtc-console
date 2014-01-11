/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QList>
#include <QSettings>

#include "MultiplexIndex.h"

MultiplexIndex::MultiplexIndex(const SystemDeviceIdentifier& id)
    : id(id), prefix(id.toString()), table(), object_size(0)
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
int MultiplexIndex::getObjectSize() const {

    return object_size;
}
void MultiplexIndex::setObjectSize(int size){

    if (0 < size){
        object_size = size;
    }
}
bool MultiplexIndex::maxObjectSize(int size){

    if (size > object_size){
        object_size = size;

        return true;
    }
    else
        return false;
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
    settings.setValue(object_size_key,object_size);
    /*
     * Index table
     */
    int count = settings.beginReadArray(prefix);
    if (0 < count){
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
    object_size = settings.value(object_size_key).toInt();
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



