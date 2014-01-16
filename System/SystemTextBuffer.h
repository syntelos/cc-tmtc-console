/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_SYSTEM_SystemTextBuffer_H
#define _CONSOLE_SYSTEM_SystemTextBuffer_H

#include <QTextStream>

/*!
 * 
 */
class SystemTextBuffer : public QTextStream {

 public:
    SystemTextBuffer();
    ~SystemTextBuffer();
    /*!
     * Set integer base to sixteen
     */
    SystemTextBuffer& hex();
    /*!
     * Set integer base to ten
     */
    SystemTextBuffer& dec();
    /*!
     * Return internal data buffer
     */
    const char* data();
    /*!
     * Clear internal data buffer
     */
    SystemTextBuffer& clear();
};
#endif
