/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <string.h>
#include <iostream>
#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include "System/SystemDeviceIdentifier.h"
#include "TMTC/TMTCName.h"
#include "Multiplex/MultiplexIndex.h"


const static char* TEST_NAMES[] = {
    "VAC",
    "SP1",
    "SP2"
};
const static int TEST_NAMES_COUNT = 3;

const static QByteArray SID("192.168.2.2:10001");

static QTextStream sout(stdout);
static QTextStream serr(stderr);

int main(int argc, char** argv){

    QCoreApplication::setOrganizationName("syntelos");
    QCoreApplication::setOrganizationDomain("syntelos.com");
    QCoreApplication::setApplicationName("tmtc-console");

    QApplication a(argc, argv);

    QString prog_name(argv[0]);

    SystemDeviceIdentifier sid(SID);

    QFile file(sid.toString("table"));

    MultiplexIndex index(sid,file);

    if (index.hasStorage()){

        int failures = 0;

        if (1 < argc && QString(argv[1]) == "--list"){

            QList<TMTCName> list = index.list();
            const int count = list.size();
            int cc;
            for (cc = 0; cc < count; cc++){

                TMTCName name = list.at(cc);

                sout << name.toString() << endl;
            }
        }
        else {

            int cc;
            for (cc = 0; cc < TEST_NAMES_COUNT; cc++){

                TMTCName name(TEST_NAMES[cc]);

                int ix = index.index(name);

                if (-1 < ix){

                    sout << "OK " << name.toString() << " [index] " << ix << endl;
                }
                else {
                    failures += 1;

                    sout << "ER " << name.toString() << " [index] " << ix << endl;
                }
            }

            for (cc = 0; cc < TEST_NAMES_COUNT; cc++){

                TMTCName name(TEST_NAMES[cc]);

                int ix = index.query(name);

                if (-1 < ix){

                    sout << "OK " << name.toString() << " [query] " << ix << endl;
                }
                else {
                    failures += 1;

                    sout << "ER " << name.toString() << " [query] " << ix << endl;
                }
            }
        }
        return failures;
    }
    else {
        serr << prog_name << ": error opening file " << file.fileName() << endl;

        return 1;
    }
}
