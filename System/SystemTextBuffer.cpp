/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QByteArray>
#include <QString>

#include "SystemTextBuffer.h"

SystemTextBuffer::SystemTextBuffer()
    : QTextStream(new QString())
{
}
SystemTextBuffer::~SystemTextBuffer()
{
    QString* string = this->string();
    if (string){
        delete string;
    }
}
SystemTextBuffer& SystemTextBuffer::hex(){

    setIntegerBase(16);
    return *this;
}
SystemTextBuffer& SystemTextBuffer::dec(){

    setIntegerBase(10);
    return *this;
}
const char* SystemTextBuffer::data(){

    QString* string = this->string();
    if (string){
        QByteArray bary = string->toAscii();
        return bary.constData();
    }
    else
        return 0;
}
SystemTextBuffer& SystemTextBuffer::clear(){

    QString* string = this->string();
    if (string){
        string->clear();
    }
    return *this;
}
