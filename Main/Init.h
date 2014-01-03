/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MAIN_Init_H
#define _CONSOLE_MAIN_Init_H

#include <QList>
#include <QObject>

#include "Configuration/Configuration.h"
#include "Multiplex/Multiplex.h"

/*!
 * Assemble a list of available services from the set coded into the
 * init proceedure (run method), and define status as UP when the
 * procedure completes successfully subject to the state of the
 * configuration.
 * 
 * STATUS UP
 * 
 * The CCDB and HCDB must be READY and UP, respectively.  The HCDB
 * must have configured devices, and each must successfully initialize
 * a Device Connection.
 * 
 * In this case, a multiplex is created and configured to each
 * connection.
 * 
 * LIST
 * 
 * The list structure contains the Configuration object, and the
 * following contributions to the UP state: CCDB, HCDB, and each
 * Device object.
 * 
 * If any device fails to configure a connection, each of the
 * previously configured devices have their connections shutdown and
 * are removed from the list.
 * 
 * When there are no devices, or no devices with connections, the list
 * size will be three and the status will be down.
 */
class Init : public QList<QObject*> {

 public:
    /*!
     */
    enum Status {
        Up, Down
    };
 private:
    /*!
     */
    Configuration* configuration;
    /*!
     */
    Status status;
    /*!
     */
    Multiplex* multiplex;

 public:
    /*!
     */
    Init(Configuration* configuration);
    /*!
     */
    ~Init();
    /*!
     * Status is UP
     */
    bool isUp() const ;
    /*!
     * Status is DOWN
     */
    bool isDown() const ;
    /*!
     * Init proceedure establishes status, returns (UP == STATUS).
     */
    bool run();
    /*!
     */
    Status getStatus() const ;
    /*!
     * With status UP, this is non-null. 
     */
    Multiplex* getMultiplex() const ;

};
#endif
