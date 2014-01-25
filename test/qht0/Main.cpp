/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <iostream>
#include <QHash>
#include "System/SystemDeviceIdentifier.h"

int main(int argc, char** argv){
    int cc;

    QHash<SystemDeviceIdentifier,QString*> qh;

    /*
     * Create
     */
    for (cc = 1; cc < argc; cc++){

        QString* string = new QString(argv[cc]);

        SystemDeviceIdentifier sid(*string);

        qh[sid] = string;
    }

    if ((argc-1) != qh.size()){

        std::cerr << argv[0] << ": error in QH SIZE (" << qh.size() << " != " << (argc-1) << ")" << std::endl;
        return 1;
    }

    /*
     * Validate
     */
    for (cc = 1; cc < argc; cc++){

        QString string(argv[cc]);

        SystemDeviceIdentifier sid(string);

        if (qh.contains(sid)){

            QString* test = qh[sid];

            if (test){

                if (string != *test){

                    std::cerr << argv[0] << ": error in QH VALIDATE [TEST 2] (" << cc << ") (QH SIZE: " << qh.size() << ")" << std::endl;
                    return 1;
                }
            }
            else {
                std::cerr << argv[0] << ": error in QH VALIDATE [TEST] (" << cc << ") (QH SIZE: " << qh.size() << ")" << std::endl;
                return 1;
            }
        }
        else {
            std::cerr << argv[0] << ": error in QH VALIDATE [CONTAINS] (" << cc << ") (QH SIZE: " << qh.size() << ")" << std::endl;
            return 1;
        }
    }

    std::cerr << argv[0] << ": successfully passed test (QH SIZE: " << qh.size() << ")" << std::endl;
    return 0;
}
