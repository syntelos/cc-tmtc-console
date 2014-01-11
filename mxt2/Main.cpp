/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <string.h>
#include <iostream>
#include <QByteArray>
#include <QFile>
#include <QIODevice>

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


int main(int argc, char** argv){

    int cc;
    for (cc = 0; cc < TEST_MSGS_COUNT; cc++){
        std::cout << TEST_MSGS[cc] << std::endl;
    }
    return 0;
}
