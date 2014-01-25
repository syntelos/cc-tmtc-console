/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#include <string.h>
#include <iostream>
#include <QFile>
#include <QIODevice>

#include "MultiplexRecord.h"

int main(int argc, char** argv){
    const int N = 3;
    const int PC = MultiplexRecord::SizeOf(N);
    const int Z = 1024;
    const qint64 FZ = (Z*PC);

    if (1 < argc){
        const char* filename = argv[1];
        bool listing = false;
        if (2 < argc){
            listing = (0 == strcmp("-l",argv[2]));
        }

        QFile file(filename);

        if (file.open(QIODevice::ReadWrite) && file.resize(FZ)){

            file.seek(0);

            void* top = file.map(0,FZ);

            file.close();

            if (top){

                std::cerr <<  "N: " << N << std::endl;
                std::cerr <<  "P: " << PC << std::endl;
                std::cerr <<  "Z: " << Z << std::endl;
                std::cerr <<  "F: " << FZ << " = (Z: " << Z << ") * (P: " << PC << ")" <<std::endl;

                void* p;
                int cc;
                /*
                 * Conditional Review
                 */
                if (listing){
                    MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(top);
                    if (r->test()){
                        for (p = top, cc = 0; cc < Z; cc++, p += PC){
                            MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(p);

                            r->print();
                        }
                    }
                    return 0;
                }
                /*
                 * Write test
                 */
                for (p = top, cc = 0; cc < Z; cc++, p += PC){

                    MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(p);

                    r->init(N);
                }
                /*
                 * Read test
                 */
                for (p = top, cc = 0; cc < Z; cc++, p += PC){

                    MultiplexRecord* r = reinterpret_cast<MultiplexRecord*>(p);

                    if (r->test()){

                        if (!r->check(N)){

                            std::cerr <<  argv[0] << ": error reading " << cc << std::endl;
                            return 1;
                        }
                    }
                    else {
                        std::cerr <<  argv[0] << ": error testing " << cc << std::endl;
                        return 1;
                    }
                }
                std::cerr <<  argv[0] << ": OK " << Z << std::endl;
                return 0;
            }
            else {
                std::cerr <<  argv[0] << ": error mapping (1MB) " << filename << std::endl;
                return 1;
            }
        }
        else {
            std::cerr <<  argv[0] << ": error resizing & opening " << filename << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "Usage: " << argv[0] << " <filename> " << std::endl;
        return 1;
    }
}
