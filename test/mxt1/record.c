/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "record.h"

size_t record_sizeof(uint8_t dc){
    return (SizeRecordBase + (dc * SizeFieldF));
}
int record_init(record* r, uint8_t dc){
    if (0 != r){
        r->gs = GS;
        r->rs = RS;

        field_L_init(&r->time);

        field_B_init(&r->count);

        r->count.value = dc;

        uint8_t cc;
        for (cc = 0; cc < dc; cc++){

            if (!field_F_init(&(r->data[cc]))){

                return -1;
            }
        }
        return 0;
    }
    else
        return -1;
}
int record_check(record* r, uint8_t dc){
    if (0 != r && GS == r->gs && RS == r->rs &&
        field_L_check(&r->time) &&
        field_B_validate(&r->count,dc))
    {
        uint8_t cc;
        for (cc = 0; cc < dc; cc++){

            if (!field_F_check(&(r->data[cc]))){

                return -1;
            }
        }
        return 0;
    }
    else
        return -1;
}
