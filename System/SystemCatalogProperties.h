/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemCatalogProperties_H
#define _CONSOLE_SYSTEM_SystemCatalogProperties_H

#include <QMap>
#include <QString>
#include <QVariant>

#include "SystemCatalog.h"

/*!
 * An instance of a subclass (\class SystemCatalogInput or \class
 * SystemCatalogOutput) is passed into the catalog I/O methods of
 * (\class SystemCatalogNode).
 * 
 * \sa SystemCatalog
 */
class SystemCatalogProperties : public QMap<SystemCatalog::Property,QVariant> {

 public:
    /*!
     */
    SystemCatalogProperties();
    /*!
     */
    SystemCatalogProperties(SystemCatalog::Property n, QVariant v);
    /*!
     */
    ~SystemCatalogProperties();

 private:
    Q_DISABLE_COPY(SystemCatalogProperties);
};
#endif
