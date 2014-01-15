/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <iostream>
#include <QHash>
#include "TMTC/TMTCName.h"

int main(int argc, char** argv){
    int cc;

    QHash<TMTCName,QString*> qh;

    /*
     * Create
     */
    for (cc = 1; cc < argc; cc++){

        QString* string = new QString(argv[cc]);

        TMTCName name(*string);

        qh[name] = string;
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

        TMTCName name(string);

        if (qh.contains(name)){

            QString* test = qh[name];

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
