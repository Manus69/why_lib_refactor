#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "declarations.h"

struct HashTable
{
    Block*  table;
    Uint    (*hash_function)(const void *);
    Uint    n_items;
};


#endif