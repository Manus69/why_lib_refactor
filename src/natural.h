#ifndef NATURAL_H
#define NATURAL_H

#include <stdint.h>

struct Natural
{
    uint32_t*   digits;
    uint32_t    n_digits;
    uint32_t    capacity;
};

#endif