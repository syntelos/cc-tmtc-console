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

    MultiplexIndex index(sid);

    int cc;
    for (cc = 0; cc < TEST_NAMES_COUNT; cc++){

        TMTCName name(TEST_NAMES[cc]);

        index[name] = cc;
    }

    for (cc = 0; cc < TEST_NAMES_COUNT; cc++){

        TMTCName name(TEST_NAMES[cc]);

        bool c = index.contains(name);

        int i = index[name];

        sout << name.toString() << ": (" << c << ") " << i << endl;
    }

    return 0;
}
