/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

#include "MultiplexSelect.h"

const qreal MultiplexSelect::InitMin = +1e127;
const qreal MultiplexSelect::InitMax = -1e127;

MultiplexSelect::MultiplexSelect(const qint64 start, const qint64 end, 
                                 const SystemDeviceIdentifier& id, const SystemName& n)
    : QPainterPath(), window(), start(start), end(end), id(id), name(n), 
      local_min_x(InitMin), local_min_y(InitMin), local_max_x(InitMax), local_max_y(InitMax), 
      global_min_x(InitMin), global_min_y(InitMin), global_max_x(InitMax), global_max_y(InitMax)
{
}
MultiplexSelect::~MultiplexSelect(){
}
bool MultiplexSelect::intersects(MultiplexObject& table) const {

    if (table.contains(this->name)){

        qint64 tt = table.getTime();

        return (this->start <= tt && tt <= this->end);
    }
    else {
        return false;
    }
}
void MultiplexSelect::append(MultiplexObject& table){

    if (this->intersects(table)){

        QVariant value = table.getValue(this->name);

        qreal x = (qreal)(table.getTime()-this->start);

        bool ok;
        qreal y = value.toReal(&ok);
        if (ok){

            if (x < local_min_x){

                local_min_x = x;
            }
            else if (x > local_max_x){

                local_max_x = x;
            }

            if (y < local_min_y){

                local_min_y = y;
            }
            else if (y > local_max_y){

                local_max_y = y;
            }

            QPointF p(x,y);

            if (this->isEmpty()){

                QPainterPath::moveTo(p);
            }
            else {

                QPainterPath::lineTo(p);
            }
        }
    }
}
MultiplexSelect& MultiplexSelect::operator+=(MultiplexObject& table){

    this->append(table);

    return *this;
}
const QRectF& MultiplexSelect::getVisualizationWindow() const {

    return this->window;
}
qreal MultiplexSelect::getVisualizationScaleX() const {

    return this->sx;
}
qreal MultiplexSelect::getVisualizationScaleY() const {

    return this->sy;
}
void MultiplexSelect::visualization(const QRectF& window, qreal sx, qreal sy){

    QPainterPath viz;

    this->sx = sx;
    this->sy = sy;

    this->window.setX(window.x());
    this->window.setY(window.y());
    this->window.setWidth(window.width());
    this->window.setHeight(window.height());

    const qreal origin_y = window.height();

    const int count = this->elementCount();
    int cc;
    for (cc = 0; cc < count; cc++){

        const QPainterPath::Element& pathel = this->elementAt(cc);

        qreal x = (pathel.x * sx);
        qreal y = (origin_y - (pathel.y * sy));

        if (0 == cc){

            viz.moveTo(x,y);
        }
        else {

            viz.lineTo(x,y);
        }
    }
    this->swap(viz);
}
