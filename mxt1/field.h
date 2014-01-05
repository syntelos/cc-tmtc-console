/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_MX_FIELD_H
#define _CONSOLE_MX_FIELD_H

#include <stdint.h>
#include <sys/types.h>

#include "seps.h"

typedef struct {
    uint8_t fs;
    uint8_t value;
} field_B;

#define SizeFieldB sizeof(field_B)

typedef struct {
    uint8_t fs;
    uint64_t value;
} field_L;

#define SizeFieldL sizeof(field_L)

typedef struct {
    uint8_t fs;
    double value;
} field_F;

#define SizeFieldF sizeof(field_F)

int field_B_init(field_B* f);
int field_B_check(field_B* f);
int field_B_validate(field_B* f, uint8_t value);

int field_L_init(field_L* f);
int field_L_check(field_L* f);
int field_L_validate(field_L* f, uint64_t value);

int field_F_init(field_F* f);
int field_F_check(field_F* f);
int field_F_validate(field_F* f, double value);

#endif
