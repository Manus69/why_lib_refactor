#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "definitions.h"

void*       MemExpand(void* memory, Uint size, Uint extra_size);
void        MemDestroy(void* ptr);
Deck*       DeckCreatePtrCapacity(void* (*copy)(const void *), void (*destroy)(void *), Uint capacity);
Deck*       DeckCreateUintCapacity(Uint capacity);
ssize_t     BlockReadFromFile(Block* block, Uint index, int file, Uint n_bytes);
Int         MatrixUnitInit(Uint r0_size, Uint r1_size, Uint row_reg_size);
void        MatrixUnitTerminate(void);

#endif