/*
 * Copyright 2014 John Pritchard, Syntelos.  All rights reserved.
 */

#include "field.h"
/*
 * field_B
 */
inline int field_B_init(field_B* f){
    if (0 != f){
        f->fs = FS;
        f->value = 0;
        return 1;
    }
    else
        return 0;
}
inline int field_B_check(field_B* f){
    return (0 != f && FS == f->fs);
}
inline int field_B_validate(field_B* f, uint8_t value){
    return (0 != f && FS == f->fs && value == f->value);
}
/*
 * field_L
 */
inline int field_L_init(field_L* f){
    if (0 != f){
        f->fs = FS;
        f->value = 0;
        return 1;
    }
    else
        return 0;
}
inline int field_L_check(field_L* f){
    return (0 != f && FS == f->fs);
}
inline int field_L_validate(field_L* f, uint64_t value){
    return (0 != f && FS == f->fs && value == f->value);
}
/*
 * field_F
 */
inline int field_F_init(field_F* f){
    if (0 != f){
        f->fs = FS;
        f->value = 0;
        return 1;
    }
    else
        return 0;
}
inline int field_F_check(field_F* f){
    return (0 != f && FS == f->fs);
}
inline int field_F_validate(field_F* f, double value){
    return (0 != f && FS == f->fs && value == f->value);
}
