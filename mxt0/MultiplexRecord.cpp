/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <iostream>

#include "MultiplexRecord.h"

int MultiplexRecord::SizeOf(int count){

    return (MX::RecordBase + (count * MX::FieldSizeF));
}
void MultiplexRecord::init(const quint8 count){
    this->gs = MX::GS;
    this->rs = MX::RS;
    this->time.init(0);
    this->count.init(count);
    int cc;
    for (cc = 0; cc < count; cc++){
        MultiplexFieldF& f = this->data[cc];
        f.init(0);
    }
}
inline bool MultiplexRecord::test(){
    return (MX::GS == gs && MX::RS == rs &&
            time.check() && count.check());
}
bool MultiplexRecord::check(){

    return MultiplexRecord::check(this->count.value);
}
bool MultiplexRecord::check(const quint8 count){

    if (MultiplexRecord::test() && count == this->count.value){

        int cc;
        for (cc = 0; cc < count; cc++){
            MultiplexFieldF& f = this->data[cc];
            if (!f.check()){
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}
void MultiplexRecord::print(){
    if (this->test()){
        std::cout << "<GS><RS> ";
        time.print();
        count.print();

        const quint8 count = this->count.value;

        int cc;
        for (cc = 0; cc < count; cc++){
            MultiplexFieldF& f = this->data[cc];
            f.print();
        }

        std::cout << std::endl;
    }
}
/*
 * MultiplexFieldL
 */
void MultiplexFieldL::init(const qint64 value){
    this->fs = MX::FS;
    this->value = value;
}
inline bool MultiplexFieldL::validate(const qint64 value){
    return (MX::FS == this->fs && value == this->value);
}
inline bool MultiplexFieldL::check(){
    return (MX::FS == this->fs);
}
void MultiplexFieldL::print(){
    if (this->check()){
        std::cout << "<FS> " << this->value << ' ';
    }
    else {
        std::cout << "<X> ";
    }
}
/*
 * MultiplexFieldF
 */
void MultiplexFieldF::init(const qreal value){
    this->fs = MX::FS;
    this->value = value;
}
inline bool MultiplexFieldF::validate(const qreal value){
    return (MX::FS == this->fs && value == this->value);
}
inline bool MultiplexFieldF::check(){
    return (MX::FS == this->fs);
}
void MultiplexFieldF::print(){
    if (this->check()){
        std::cout << "<FS> " << this->value << ' ';
    }
    else {
        std::cout << "<X> ";
    }
}
/*
 * MultiplexFieldB
 */
void MultiplexFieldB::init(const quint8 value){
    this->fs = MX::FS;
    this->value = value;
}
inline bool MultiplexFieldB::validate(const quint8 value){
    return (MX::FS == this->fs && value == this->value);
}
inline bool MultiplexFieldB::check(){
    return (MX::FS == this->fs);
}
void MultiplexFieldB::print(){
    if (this->check()){
        std::cout << "<FS> " << this->value << ' ';
    }
    else {
        std::cout << "<X> ";
    }
}
