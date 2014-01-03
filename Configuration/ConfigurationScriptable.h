/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_ConfigurationScriptable_H
#define _CONSOLE_ConfigurationScriptable_H

#include <QMetaObject>

#include "System/SystemScriptable.h"
#include "System/SystemScriptSymbol.h"
#include "Script.h"

/*!
 * 
 */
class ConfigurationScriptable : public SystemScriptable {

 public:
    /*!
     */
    ConfigurationScriptable();
    /*!
     */
    ~ConfigurationScriptable();
    /*!
     * Called by the subclass to connect targets to link source
     * signals.  
     */
    void initConfigurationScriptable(QObject* subclass);

};

#endif
