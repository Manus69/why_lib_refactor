#ifndef BLOCK_H
#define BLOCK_H

#include "why.h"

struct Block
{
    void*                   memory;
    const TypeInterface*    interface;
    Uint                    n_items;
};

#endif