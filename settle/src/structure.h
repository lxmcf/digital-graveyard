#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "defines.h"

typedef struct Structure {
    string texture_key;

    struct {
        uint16_t x;
        uint16_t y;
    } current_cell;

} Structure;

#endif // STRUCTURE_H
