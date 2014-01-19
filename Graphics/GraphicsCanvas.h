/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_GRAPHICS_GraphicsCanvas_H
#define _CONSOLE_GRAPHICS_GraphicsCanvas_H

#include <QGraphicsView>
#include <QScriptEngine>
#include <QWidget>

#include "Configuration/ConfigurationScriptable.h"
#include "GraphicsScene.h"
#include "GraphicsBranch.h"

/*!
 *
 */
class GraphicsCanvas : public QGraphicsView,
    public SystemCatalogNode,
    public ConfigurationScriptable
{
    Q_OBJECT;

 public:
    static void InitScriptMetaType(QScriptEngine* engine);

    GraphicsCanvas(QWidget* parent = 0);
    ~GraphicsCanvas();

    GraphicsScene* scene();

    GraphicsScene& add(GraphicsBranch*);

    GraphicsScene& debugPrintChildren();

 public slots:
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
    Q_DISABLE_COPY(GraphicsCanvas);
};

Q_DECLARE_METATYPE(GraphicsCanvas*)
#endif
