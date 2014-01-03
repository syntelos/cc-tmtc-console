/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _SYSTEM_SystemWindow_H
#define _SYSTEM_SystemWindow_H

#include <QMenu>
#include <QMenuBar>
#include <QString>

/*!
 * This interface is implemented by the (application main) "window"
 * object class, and some dialog windows.
 */
class SystemWindow {

 public:
    /*!
     * Create contextual menu
     * 
     * TODO Review Qt Contextual Menu
     */
    QMenu* addMenu(const QString&);
    /*!
     * Remove contextual menu
     * 
     * TODO Review Qt Contextual Menu
     */
    void removeMenu(const QString&);

};

#endif
