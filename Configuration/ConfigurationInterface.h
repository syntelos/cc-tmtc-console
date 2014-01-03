/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_ConfigurationInterface_H
#define _CONSOLE_CONFIGURATION_ConfigurationInterface_H

#include "CCDB.h"
#include "HCDB.h"

/*!
 * Interface defined by members of the configuration tree.
 */
class ConfigurationInterface  {

 public:
    /*!
     * Derive CCDB from the configuration tree.
     */
    virtual CCDB* getCCDB() = 0;
    /*!
     * Derive HCDB from the configuration tree.
     */
    virtual HCDB* getHCDB() = 0;

};

#endif
