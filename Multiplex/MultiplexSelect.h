/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_DATABASE_MultiplexSelect_H
#define _CONSOLE_DATABASE_MultiplexSelect_H

#include <QList>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QVariant>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCName.h"
#include "MultiplexTable.h"

/*!
 * A visual selection result is a plot path.
 */
class MultiplexSelect : public QPainterPath {
    /*!
     * Visualization parameters
     */
    QRectF window;

    qreal sx;

    qreal sy;

 public:
    const static qreal InitMin;
    const static qreal InitMax;
    /*!
     * Selection parameters
     * 
     * Absolute domain origin
     */
    const qint64 start;
    /*!
     * Absolute domain limit
     */
    const qint64 end;
    /*!
     */
    const SystemDeviceIdentifier& id;
    /*!
     */
    const TMTCName& name;
    /*!
     * In the construction phase of selection, these values are local
     * to this selection set.
     */
    qreal local_min_x;
    qreal local_min_y;
    qreal local_max_x;
    qreal local_max_y;
    /*!
     * Following the visualization phase of selection, these values
     * are global to the whole selection set requested of the \class
     * Multiplex.
     */
    qreal global_min_x;
    qreal global_min_y;
    qreal global_max_x;
    qreal global_max_y;
    /*!
     */
    MultiplexSelect(const qint64 start, const qint64 end,
                    const SystemDeviceIdentifier& id, const TMTCName& n);
    /*!
     */
    ~MultiplexSelect();
    /*!
     * Return the value of the argument to the visualization
     * proceedure
     */
    const QRectF& getVisualizationWindow() const ;
    /*!
     */
    qreal getVisualizationScaleX() const ;
    /*!
     */
    qreal getVisualizationScaleY() const ;
    /*!
     * Called by 'append' to test for intersection with this selection.
     */
    bool intersects(MultiplexTable&) const ;
    /*!
     * If this selection 'contains' the argument, then append the
     * coordinate into a normalized domain for the path.
     */
    void append(MultiplexTable&);
    /**
     * After the path result has been constructed, the application of
     * this proceedure is required to prepare the path result for
     * display.
     * 
     * AUTOSCALING
     * 
     * Normalize each Y coordinate by subtracting the 'min_y' value.
     * 
     * Flip each Y coordinate by subtraction from the argument value.
     */
    void visualization(const QRectF& window, qreal sx, qreal sy);

    MultiplexSelect& operator+=(MultiplexTable& table);

};
#endif
