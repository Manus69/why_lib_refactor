#ifndef BLOCK_H
#define BLOCK_H

#include "why.h"

typedef struct Block Block;

struct Block
{
    void*   memory;
    Uint    n_items;
    Uint    item_size;

    void    (*get)(void* target, const Block* block, Uint index);
    void    (*swap)(Block* block, Uint j, Uint k);
};

#define     BlockInit(block, n_items, type) BlockInit ## type(block, n_items)
Int         BlockInitInt(Block* block, Uint n_items);
Int         BlockInitUint(Block* block, Uint n_items);
Int         BlockInitFloat(Block* block, Uint n_items);
Int         BlockInitRational(Block* block, Uint n_items);
Int         BlockInitComplex(Block* block, Uint n_items);
Int         BlockInitPtr(Block* block, Uint n_items);

Uint        BlockGetSize(const Block* block);
Uint        BlockGetNItems(const Block* block);
Int         BlockExpand(Block* block, Uint extra_items);
void        BlockDestroy(Block* block);
void        BlockGet(void* target, const Block* block, Uint index);
void        BlockSwap(Block* block, Uint j, Uint k);

Int         BlockGetInt(const Block* block, Uint index);
Uint        BlockGetUint(const Block* block, Uint index);
Float       BlockGetFloat(const Block* block, Uint index);
Complex     BlockGetComplex(const Block* block, Uint index);
Rational    BlockGetRational(const Block* block, Uint index);
void*       BlockGetPtr(const Block* block, Uint index);

#define     BlockSet(block, index, item) _Generic((item), \
                                            Int : BlockSetInt, \
                                            Uint : BlockSetUint, \
                                            Float : BlockSetFloat, \
                                            Rational : BlockSetRational, \
                                            Complex : BlockSetComplex, \
                                            default : BlockSetPtr)(block, index, item)
void        BlockSetInt(Block* block, Uint index, Int value);
void        BlockSetUint(Block* block, Uint index, Uint value);
void        BlockSetFloat(Block* block, Uint index, Float x);
void        BlockSetComplex(Block* block, Uint index, Complex z);
void        BlockSetRational(Block* block, Uint index, Rational p);
void        BlockSetPtr(Block* block, Uint index, void* ptr);

void        BlockGetIntWRAP(void* target, const Block* block, Uint index);
void        BlockGetUintWRAP(void* target, const Block* block, Uint index);
void        BlockGetFloatWRAP(void* target, const Block* block, Uint index);
void        BlockGetRationalWRAP(void* target, const Block* block, Uint index);
void        BlockGetComplexWRAP(void* target, const Block* block, Uint index);
void        BlockGetPtrWRAP(void* target, const Block* block, Uint index);

void        BlockSwapInt(Block* block, Uint j, Uint k);
void        BlockSwapUint(Block* block, Uint j, Uint k);
void        BlockSwapFloat(Block* block, Uint j, Uint k);
void        BlockSwapRational(Block* block, Uint j, Uint k);
void        BlockSwapComplex(Block* block, Uint j, Uint k);
void        BlockSwapPtr(Block* block, Uint j, Uint k);

#endif