#ifndef __TEST_H
#define __TEST_H

#include "user.h"

#define assert(x) \
    do { \
        if (!(x)) { \
            printf("Assertion failed: %s, file %s, line %d\n", #x, __FILE__, __LINE__); \
            exit(-1); \
        } \
    } while (0)

#endif
