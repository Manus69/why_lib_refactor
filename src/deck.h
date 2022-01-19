#ifndef DECK_H
#define DECK_H

#include "why.h"

#define DECK_CAPACITY (1 << 2)

struct Deck
{
    Block*  block;
    void*   (*copy)(const void* );
    void    (*destroy)(void *);

    Uint    left_insert_index;
    Uint    right_insert_index;
};

#endif