/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_ERROR_H
#define _CONSOLE_CONFIGURATION_ERROR_H

namespace ConfigurationError {
    /*!
     * Constructor exception thrown for a failure to open a database.
     */
    class DatabaseOpen {
     public:
        const char* component;

        DatabaseOpen(const char* component)
            : component(component)
        {
        }
    };
    /*!
     * Constructor exception thrown for a failure to use a database
     * driver.
     */
    class DatabaseDriver {
     public:
        const char* component;

        DatabaseDriver(const char* component)
            : component(component)
        {
        }
    };
    /*!
     * Constructor exception thrown for a failure to create a table.
     */
    class DatabaseInitTable {
     public:
        const char* component;

        DatabaseInitTable(const char* component)
            : component(component)
        {
        }
    };
    /*!
     * Constructor exception thrown for a failure to insert a row.
     */
    class DatabaseInitRecord {
     public:
        const char* component;

        DatabaseInitRecord(const char* component)
            : component(component)
        {
        }
    };
    /*!
     * Constructor exception thrown for a failure in reading the
     * database table.
     */
    class DatabaseRead {
     public:
        const char* component;

        DatabaseRead(const char* component)
            : component(component)
        {
        }
    };
    /*!
     * Constructor exception thrown for a failure in writing the
     * database table.
     */
    class DatabaseWrite {
     public:
        const char* component;

        DatabaseWrite(const char* component)
            : component(component)
        {
        }
    };

}

#endif
