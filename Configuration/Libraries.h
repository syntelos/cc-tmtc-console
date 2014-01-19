/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Libraries_H
#define _CONSOLE_CONFIGURATION_Libraries_H

#include <QMetaProperty>
#include <QObject>
#include <QScriptEngine>
#include <QUuid>
#include <QWidget>

#include "ObjectTree/ObjectTreeNode.h"
#include "ObjectTree/ObjectTreeList.h"
#include "System/SystemCatalogNode.h"
#include "Library.h"

class Libraries : public ObjectTreeNode,
    public ObjectTreeList,
    public SystemCatalogNode
{
    Q_OBJECT;


 public:
    static void InitScriptMetaType(QScriptEngine* engine);
    /*!
     */
    Libraries(QObject* parent);
    /*!
     */
    ~Libraries();
    /*!
     * Edit node, appending a new child: return success.
     */
    virtual bool insertObjectTreeList();
    /*!
     * Edit node, removing the indexed child: return success.
     */
    virtual bool removeObjectTreeList(int idx);

 public slots:
    /*!
     */
    void clear();
    /*!
     */
    virtual void start();
    /*!
     */
    virtual void stop();
    /*!
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&);
    /*!
     */
    virtual void write(SystemCatalogOutput&, QDomElement&);


 private:
    Q_DISABLE_COPY(Libraries)
};

Q_DECLARE_METATYPE(Libraries*)
#endif
