#ifndef BLOCK_H
#define BLOCK_H

#include "why.h"

struct Block
{
    void*   memory;
    Uint    n_items;
    Uint    item_size;

    void    (*get)(void* target, const Block* block, Uint index);
    void    (*swap)(Block* block, Uint j, Uint k);
};

#endif