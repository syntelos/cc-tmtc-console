/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_GRAPHICS_GraphicsBranch_H
#define _CONSOLE_GRAPHICS_GraphicsBranch_H

#include <QEvent>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

/*!
 * A branch node in this hierarchical scene graph has \class QObject
 * membership and directs its GUI events (paint and scene) to its
 * children.
 */
class GraphicsBranch : public QGraphicsObject
{
 public:
    /*!
     */
    static const char* GraphicsClassNameUnknown;
    static const char* GraphicsClassNameUnknownBorder;
    /*!
     */
    GraphicsBranch(QGraphicsItem* parent = 0);
    /*!
     */
    ~GraphicsBranch();
    /*!
     */
    virtual const char* graphicsClassName() = 0;
    /*!
     */
    virtual const char* describeClassName(QGraphicsItem* item) = 0;
    /*!
     */
    virtual QRectF boundingRect() const;
    /*!
     */
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    /*!
     */
    virtual bool sceneEvent(QEvent* event);

 private:
    Q_DISABLE_COPY(GraphicsBranch)
};
#endif
