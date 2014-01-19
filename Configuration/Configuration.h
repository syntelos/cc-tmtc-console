/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Configuration_H
#define _CONSOLE_CONFIGURATION_Configuration_H

#include <QNetworkAccessManager>
#include <QScriptEngine>

#include "Devices.h"
#include "Libraries.h"
#include "Scripts.h"

/*!
 * Software system tree root.
 * 
 * \sa Window
 */
class Configuration {

 public:
    /*!
     */
    virtual QNetworkAccessManager* getNetworkManager() = 0;
    /*!
     */
    virtual QScriptEngine* getScriptEngine() const = 0;
    /*!
     */
    virtual Libraries* getLibraries() const = 0;
    /*!
     */
    virtual Devices* getDevices() const = 0;
    /*!
     */
    virtual Scripts* getScripts() const = 0;

};
#endif
