/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_Configuration_H
#define _CONSOLE_Configuration_H

#include <QList>
#include <QModelIndex>
#include <QObject>
#include <QScriptable>
#include <QScriptEngine>
#include <QUuid>
#include <QVariant>

#include "CCDB.h"
#include "HCDB.h"
#include "Devices.h"
#include "Libraries.h"
#include "ObjectTree/ObjectTreeModel.h"
#include "Storage/StorageTreeEditor.h"
#include "System/SystemScriptable.h"
#include "Null.h"

/*!
 * Root of the configuration tree, scriptable "configuration".
 * 
 * The \class QObject tree parent of the primary instance of this type
 * is defined to one unique view that the \class ObjectTreeModel
 * depends on.  This idea would need to be revised for multiple views
 * on this object, but that's not a planned use case.
 */
class Configuration : public ObjectTreeModel,
    public SystemScriptable
{
    Q_OBJECT;
    Q_PROPERTY(CCDB* ccdb READ getCCDB USER false FINAL);
    Q_PROPERTY(HCDB* hcdb READ getHCDB USER false FINAL);

    /*!
     */
    QScriptEngine* engine;
    /*!
     */
    CCDB* ccdb;
    /*!
     */
    HCDB* hcdb;
    /*!
     */
    Configuration(QScriptEngine* engine);
    ~Configuration();

 public:
    /*!
     * Typical access
     */
    static Configuration* Instance();
    /*!
     * Special access for Main/Window.
     */
    static Configuration* Init(QScriptEngine* engine);
    /*!
     */
    CCDB* getCCDB() const;
    /*!
     */
    HCDB* getHCDB() const;
    /*!
     */
    QScriptEngine* getScriptEngine() const;

 public slots:
    /*!
     */
    void configureWindowInit();
    /*!
     */
    void deconfigureWindowInit();


 private:
    Q_DISABLE_COPY(Configuration)
};

#endif
