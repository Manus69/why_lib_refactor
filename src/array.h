#ifndef ARRAY_H
#define ARRAY_H

#include "why.h"
#include "block.h"

struct Array
{
    Block   block;
    Uint    left_insert_index;
    Uint    right_insert_index;

    void*   (*copy)(void* item);
    void    (*destroy)(void* item);
};

#endif