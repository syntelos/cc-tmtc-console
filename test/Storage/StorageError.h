/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */

namespace StorageError {
    /*!
     * Exception thrown from constructor when a user property has no
     * editor.  The subject class name and property name are
     * identified in the exception.
     */
    class EditorNotFoundFor {

     public:
        const char* component;
        const char* property;

        EditorNotFoundFor(const char* component, const char* property)
            : component(component), property(property)
        {
        }
    };
}
