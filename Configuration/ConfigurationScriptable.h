/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_ConfigurationScriptable_H
#define _CONSOLE_ConfigurationScriptable_H

#include <QMetaObject>

#include "System/SystemScriptable.h"
#include "System/SystemScriptSymbol.h"
#include "Configuration.h"
#include "Script.h"

/*!
 * The implementor must be a member of class \class QObject.
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
     * Called by the subclass to connect (link) target slots to (link)
     * source signals.  The argument is "this".
     */
    void initConfigurationScriptable(QObject*);
    /*!
     * Walk the system tree to the node which implements the \class
     * Configuration interface.  The argument is "this".
     */
    Configuration* configuration(QObject*);
};

#endif
