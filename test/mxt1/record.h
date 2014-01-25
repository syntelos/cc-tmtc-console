/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MX_RECORD_H
#define _CONSOLE_MX_RECORD_H

#include <stdint.h>
#include <sys/types.h>

#include "seps.h"
#include "field.h"

typedef struct {
    uint8_t gs;
    uint8_t rs;
    field_L time;
    field_B count;
    field_F data[];
} record;

#define SizeRecordBase (2+SizeFieldL+SizeFieldB)

size_t record_sizeof(uint8_t dc);

int record_init(record* r, uint8_t dc);

int record_check(record* r, uint8_t dc);


#endif
