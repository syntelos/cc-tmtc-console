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
#include "TMTC/TMTCMessage.h"
#include "Multiplex/MultiplexTable.h"


const static char* TEST_MSGS[] = {
    "VAC=4.8648e-01 SP1=1.0000e+02 SP2=4.9999e+02", // 0
    "VAC=4.8658e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8657e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8665e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8657e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8664e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8662e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8657e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8655e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8648e-01 SP1=1.0000e+02 SP2=4.9999e+02", // 9
    "VAC=4.8646e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8655e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8660e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8653e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8650e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8656e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8657e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8658e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8671e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8664e-01 SP1=1.0000e+02 SP2=4.9999e+02", //19
    "VAC=4.8662e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8663e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8661e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8660e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8652e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8651e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8654e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8666e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8663e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8662e-01 SP1=1.0000e+02 SP2=4.9999e+02", //29
    "VAC=4.8663e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8658e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8657e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8658e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8660e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8658e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8662e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8660e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8671e-01 SP1=1.0000e+02 SP2=4.9999e+02",
    "VAC=4.8660e-01 SP1=1.0000e+02 SP2=4.9999e+02"  //39
};
const static int TEST_MSGS_COUNT = 40;

const static QByteArray SID("192.168.2.2:0");

static QTextStream sout(stdout);
static QTextStream serr(stderr);

int main(int argc, char** argv){

    QCoreApplication::setOrganizationName("syntelos");
    QCoreApplication::setOrganizationDomain("syntelos.com");
    QCoreApplication::setApplicationName("tmtc-console");

    QApplication a(argc, argv);

    QString prog_name(argv[0]);

    SystemDeviceIdentifier sid(SID);

    QString table_name = sid.toString("table");

    MultiplexTable table(sid);


    serr << prog_name << ": table '" <<  table_name << "'" << endl;

    if (table.open()){

        serr << prog_name << ": successfully opened table '" <<  table_name << "'" << endl;

        int cc;
        for (cc = 0; cc < TEST_MSGS_COUNT; cc++){

            TMTCMessage m(TEST_MSGS[cc]);

            table.update(m);
        }

        table.close();

        return 0;
    }
    else {

        serr << prog_name << ": error opening table '" <<  table_name << "'" << endl;
        return 1;
    }
}
