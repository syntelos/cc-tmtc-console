/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>

#include "SystemCatalogProperties.h"

SystemCatalogProperties::SystemCatalogProperties()
    : QMap()
{
}
SystemCatalogProperties::SystemCatalogProperties(SystemCatalog::Property n, QVariant v)
    : QMap()
{
    insert(n,v);
}
SystemCatalogProperties::~SystemCatalogProperties()
{
}
