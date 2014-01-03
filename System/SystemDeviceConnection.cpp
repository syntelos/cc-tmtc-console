/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "SystemDeviceConnection.h"

SystemDeviceConnection::SystemDeviceConnection(QObject* parent)
    : QThread(parent), shutdown_flag(false)
{
}
SystemDeviceConnection::~SystemDeviceConnection(){
}
void SystemDeviceConnection::shutdown(){
    this->shutdown_flag = true;
}
