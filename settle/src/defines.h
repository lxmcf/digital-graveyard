#ifndef DEFINES_H
#define DEFINES_H

#include <math.h>
#include <stdint.h>
#include <stddef.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// NOTE: Will return sinf but between 0 and 1
#define SINE(x) ((sinf ((float)x) + 1.0f) / 2.0f)

// NOTE: Lower case to match sizeof
#define lengthof(x) (sizeof (x) / sizeof (x[0]))

#if defined(__cplusplus)
    #define CLITERAL(type) type
#else
    #define CLITERAL(type) (type)
#endif

typedef const char* string;

typedef struct Point {
    int x;
    int y;
} Point;

#endif // DEFINES_H
