/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_GRAPHICS_GraphicsLeaf_H
#define _CONSOLE_GRAPHICS_GraphicsLeaf_H

#include <QEvent>

/*!
 * A leaf node in this hierarchical scene graph is a kind of \class
 * QGraphicsItem that has a child relation to a \class GraphicsBranch
 * node.
 */
class GraphicsLeaf {

 public:
    /*!
     */
    virtual const char* graphicsClassName() = 0;
    /*!
     */
    virtual bool sceneEvent(QEvent* event) = 0;

};
#endif
