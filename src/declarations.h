#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "why.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void*       mem_expand(void* memory, Uint size, Uint extra_size);

#define     BlockInit(block, n_items, type) BlockInit ## type(block, n_items)
Int         BlockInitByte(Block* block, Uint n_items);
Int         BlockInitInt(Block* block, Uint n_items);
Int         BlockInitUint(Block* block, Uint n_items);
Int         BlockInitFloat(Block* block, Uint n_items);
Int         BlockInitRational(Block* block, Uint n_items);
Int         BlockInitComplex(Block* block, Uint n_items);
Int         BlockInitPtr(Block* block, Uint n_items);

Byte        BlockGetByte(const Block* block, Uint index);
Int         BlockGetInt(const Block* block, Uint index);
Uint        BlockGetUint(const Block* block, Uint index);
Float       BlockGetFloat(const Block* block, Uint index);
Complex     BlockGetComplex(const Block* block, Uint index);
Rational    BlockGetRational(const Block* block, Uint index);
void*       BlockGetPtr(const Block* block, Uint index);

void        BlockGetByteWRAP(void* traget, const Block* block, Uint index);
void        BlockGetIntWRAP(void* target, const Block* block, Uint index);
void        BlockGetUintWRAP(void* target, const Block* block, Uint index);
void        BlockGetFloatWRAP(void* target, const Block* block, Uint index);
void        BlockGetRationalWRAP(void* target, const Block* block, Uint index);
void        BlockGetComplexWRAP(void* target, const Block* block, Uint index);
void        BlockGetPtrWRAP(void* target, const Block* block, Uint index);

void        BlockSwapByte(Block* block, Uint j, Uint k);
void        BlockSwapInt(Block* block, Uint j, Uint k);
void        BlockSwapUint(Block* block, Uint j, Uint k);
void        BlockSwapFloat(Block* block, Uint j, Uint k);
void        BlockSwapRational(Block* block, Uint j, Uint k);
void        BlockSwapComplex(Block* block, Uint j, Uint k);
void        BlockSwapPtr(Block* block, Uint j, Uint k);

#endif