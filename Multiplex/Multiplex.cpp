/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QList>

#include "TMTC/TMTCNameValue.h"
#include "Multiplex.h"
#include "MultiplexTable.h"

Multiplex::Multiplex(QObject* parent)
    : QObject(parent)
{
}
Multiplex::~Multiplex(){
}
bool Multiplex::update(const SystemDeviceIdentifier* id, const TMTCMessage* m){
    if (id && m){

        const SystemDeviceIdentifier& sid = *id;

        if (sid.isValid() || sid.isSpecial()){

            MultiplexTable* history = 0;

            if (this->state.contains(sid)){

                history = this->state[sid];
            }
            else {
                history = new MultiplexTable(sid);
                this->state[sid] = history;
            }
            /*
             * An invalid (special) ID will deposit values into the
             * referenced data set.  The values in the "special" data
             * set have "desktop" semantics: they cannot be proxied to
             * an Instrument Device (Connection).
             */
            history->update(*m);

            return true;
        }
    }
    return false;
}
TMTCMessage* Multiplex::query(const SystemDeviceIdentifier* id, const TMTCMessage* m){
    if (id && m){

        const SystemDeviceIdentifier& sid = *id;

        if (sid.isValid() && this->state.contains(sid)){

            MultiplexTable* history = this->state[sid];

            return history->query(*m);
        }
        else if (sid.isSpecial()){

            TMTCMessage* re = new TMTCMessage();

            if (m->isSpecial()){
                /*
                 * Use desktop semantics
                 */
                const SystemDeviceIdentifier& sid = *id;

                MultiplexTable* special = 0;

                if (this->state.contains(sid)){

                    special = this->state[sid];
                }
                else {
                    special = new MultiplexTable(sid);
                    this->state[sid] = special;
                }

                QList<TMTCNameValue*>::const_iterator p, z;
                for (p = m->constBegin(), z = m->constEnd(); p != z; ++p){
                    const TMTCNameValue* nvp = *p;

                    if (nvp->hasValue()){
                        /*
                         * Update
                         */
                        special->update(*nvp);
                    }
                    /*
                     * Query
                     */
                    const TMTCName& n = nvp->getName();
                    QVariant v = special->query(n);

                    if (v.isValid()){
                        /*
                         * Query response
                         */
                        re->append(new TMTCNameValue(n,v));
                    }
                }
            }
            else {
                /*
                 * Use broadcast semantics
                 */
                QList<SystemDeviceIdentifier> dk = state.keys();
                QList<SystemDeviceIdentifier>::const_iterator dp, dz;
                for (dp = dk.constBegin(), dz = dk.constEnd(); dp != dz; ++dp){
                    MultiplexTable* history = state[*dp];

                    QList<TMTCNameValue*>::const_iterator p, z;
                    for (p = m->constBegin(), z = m->constEnd(); p != z; ++p){
                        const TMTCNameValue* nvp = *p;

                        if (nvp->hasNotValue()){

                            const TMTCName& n = nvp->getName();
                            QVariant v = history->query(n);

                            if (v.isValid()){

                                re->append(new TMTCNameValue(n,v));
                            }
                        }
                    }
                }
            }

            return re;
        }
    }
    return 0;
}
QVariant Multiplex::query(const SystemDeviceIdentifier& d, const TMTCName& n){

    if (d.isValid() && this->state.contains(d)){

        MultiplexTable* history = this->state[d];

        return history->query(n);
    }
    else {
        QVariant nil;
        return nil;
    }
}
void Multiplex::select(int count, MultiplexSelect** query, const QRectF& window){
    if (0 < count){

        int cc;
        for (cc = 0; cc < count; cc++){

            MultiplexSelect& select = *query[cc];

            const SystemDeviceIdentifier& sid = select.id;

            if (sid.isValid() && this->state.contains(sid)){

                MultiplexTable* history = this->state[sid];

                history->select(select);
            }
        }
        /*
         * Determine boundaries of results
         */
        qreal min_x = MultiplexSelect::InitMin;
        qreal min_y = MultiplexSelect::InitMin;
        qreal max_x = MultiplexSelect::InitMax;
        qreal max_y = MultiplexSelect::InitMax;

        for (cc = 0; cc < count; cc++){

            MultiplexSelect& select = *query[cc];

            if (select.local_min_x < min_x)
                min_x = select.local_min_x;
            else if (select.local_max_x > max_x)
                max_x = select.local_max_x;

            if (select.local_min_y < min_y)
                min_y = select.local_min_y;
            else if (select.local_max_y > max_y)
                max_y = select.local_max_y;
        }


        const qreal wx = window.x();
        const qreal wy = window.y();
        const qreal ww = window.width();
        const qreal wh = window.height();
        /*
         * Autoscaling all results into one scale
         */
        const qreal sx = (ww/(max_x-min_x));
        const qreal sy = (wh/(max_y-min_y));

        for (cc = 0; cc < count; cc++){

            MultiplexSelect& select = *query[cc];

            select.global_min_x = min_x;
            select.global_max_x = max_x;
            select.global_min_y = min_y;
            select.global_max_y = max_y;

            select.visualization(window,sx,sy);
        }
    }
}
void Multiplex::receivedFromDevice(const SystemDeviceIdentifier* id, const TMTCMessage* m){
    if (id && m && this->update(id,m)){

        emit sendToUser(id,m);
    }
}
void Multiplex::receivedFromUser(const SystemDeviceIdentifier* id, const TMTCMessage* m){

    TMTCMessage* re = this->query(id,m);
    if (re){

        emit sendToUser(id,re);

        re->deleteLater();
    }
}
