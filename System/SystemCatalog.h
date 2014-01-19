/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemCatalog_H
#define _CONSOLE_SYSTEM_SystemCatalog_H

/*!
 * Definitions employed by \class SystemCatalogProperties for catalog
 * (system configuration meta data) I/O.
 *
 * \sa SystemCatalogNode
 */
namespace SystemCatalog {
    /*!
     * Canonical set of catalog I/O behavior types
     * 
     * \sa SystemCatalogNode
     */
    enum Property {
        /*!
         * Has value concrete or abstract.
         */
        PropertyConfigure,
        /*
         * Configure the system for running.  A subsequent start event
         * may be employed to direct an abstract configuration to
         * request user input required for operation.
         */
        PropertyConfigureConcrete,
        /*
         * Configure the system as directed, ignoring a runtime that
         * is incomplete with respect to typical operation.  A
         * subsequent start event is not employed to request
         * additional user input.
         */
        PropertyConfigureAbstract,
        /*!
         * Has value concrete, abstract or client.
         */
        PropertyPublish,
        /*
         * Output the complete system specification which requires no
         * additional configuration for network devices and services.
         * This output preserves the current state of the software
         * system as the original copy.
         */
        PropertyPublishConcrete,
        /*
         * Output a system model which requires the configuration of
         * connected devices and user specific cloud services.  This
         * output replicates the current state of the software system
         * as a clone that is safe for sharing.
         */
        PropertyPublishAbstract,
        /*
         * System client via HTTP.  A property with this name (key)
         * must accompany the use of this property as the value of
         * PropertyPublish, and has a string URL value for the TM/TC
         * message service (point of access).
         */
        PropertyPublishClientHttp,
        /*
         * System client via XMPP.  A property with this name (key)
         * must accompany the use of this property as the value of
         * PropertyPublish, and has a string XAddress value for the
         * TM/TC message service (point of access).
         */
        PropertyPublishClientXmpp,
    };
}
#endif
