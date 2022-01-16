#include "block.h"
#include "declarations.h"
#include "rational.h"
#include "macro.h"

static void _init(Block* block, void* memory, Uint item_size, Uint n_items, 
                    void (*get)(void* , const Block* , Uint ),
                    void (*swap)(Block* , Uint , Uint ))
{
    block->memory = memory;
    block->item_size = item_size;
    block->n_items = n_items;
    block->get = get;
    block->swap = swap;
}

static Int _create_mem(Block* block, Uint n_items, Uint item_size, 
                    void (*get)(void* , const Block* , Uint ),
                    void (*swap)(Block* , Uint , Uint ))
{
    void* memory;

    memory = malloc(n_items * item_size);

    if (!memory)
        return WHY_ERROR;
    
    _init(block, memory, item_size, n_items, get, swap);

    return WHY_OK;
}

Int BlockInitByte(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(Byte), BlockGetByteWRAP, BlockSwapByte);
}

Int BlockInitInt(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(Int), BlockGetIntWRAP, BlockSwapInt);
}

Int BlockInitUint(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(Uint), BlockGetUintWRAP, BlockSwapUint);
}

Int BlockInitFloat(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(Float), BlockGetFloatWRAP, BlockSwapFloat);
}

Int BlockInitRational(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(Rational), BlockGetRationalWRAP, BlockSwapRational);
}

Int BlockInitComplex(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(Rational), BlockGetComplexWRAP, BlockSwapComplex);
}

Int BlockInitPtr(Block* block, Uint n_items)
{
    return _create_mem(block, n_items, sizeof(void *), BlockGetPtrWRAP, BlockSwapPtr);
}

static Block* _create(Uint n_items, Int (*init)(Block* , Uint))
{
    Block* block;

    if ((block = malloc(sizeof(*block))))
    {
        if (init(block, n_items) != WHY_ERROR)
            return block;
        free(block);
    }

    return NULL;
}

Block* BlockCreateByte(Uint n_items)
{
    return _create(n_items, BlockInitByte);
}

Block* BlockCreateInt(Uint n_items)
{
    return _create(n_items, BlockInitInt);
}

Block* BlockCreateUint(Uint n_items)
{
    return _create(n_items, BlockInitUint);
}

Block* BlockCreateFloat(Uint n_items)
{
    return _create(n_items, BlockInitFloat);
}

Block* BlockCreateRational(Uint n_items)
{
    return _create(n_items, BlockInitRational);
}

Block* BlockCreateComplex(Uint n_items)
{
    return _create(n_items, BlockInitComplex);
}

Block* BlockCreatePtr(Uint n_items)
{
    return _create(n_items, BlockInitPtr);
}

Uint BlockGetSize(const Block* block)
{
    return block->n_items * block->item_size;
}

Uint BlockGetNItems(const Block* block)
{
    return block->n_items;
}

Int BlockExpand(Block* block, Uint extra_items)
{
    Uint    new_size;
    Uint    extra_size;
    Uint    old_size;
    void*   new_memory;

    old_size = BlockGetSize(block);
    extra_size = extra_items * block->item_size;
    new_size = old_size + extra_size;

    if (new_size < old_size)
        return WHY_ERROR;

    if (!(new_memory = mem_expand(block->memory, old_size, extra_size)))
        return WHY_ERROR;
    
    free(block->memory);
    block->memory = new_memory;
    block->n_items = block->n_items + extra_items;

    return WHY_OK;
}

void BlockDestroy(Block* block)
{
    free(block->memory);
    free(block);
}

void BlockGet(void* target, const Block* block, Uint index)
{
    return block->get(target, block, index);
}

void* BlockPointAt(const Block* block, Uint index)
{
    return block->memory + block->item_size * index;
}

void BlockSwap(Block* block, Uint j, Uint k)
{
    return block->swap(block, j, k);
}

Byte BlockGetByte(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, Byte);
}

Int BlockGetInt(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, Int);
}

Uint BlockGetUint(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, Uint);
}

Float BlockGetFloat(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, Float);
}

Rational BlockGetRational(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, Rational);
}

Complex BlockGetComplex(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, Complex);
}

void* BlockGetPtr(const Block* block, Uint index)
{
    return MEM_GET(block->memory, index, void *);
}

void BlockSetByte(Block* block, Uint index, Byte value)
{
    MEM_SET(block->memory, index, value, Byte);
}

void BlockSetInt(Block* block, Uint index, Int n)
{
    MEM_SET(block->memory, index, n, Int);
}

void BlockSetUint(Block* block, Uint index, Uint n)
{
    MEM_SET(block->memory, index, n, Uint);
}

void BlockSetFloat(Block* block, Uint index, Float x)
{
    MEM_SET(block->memory, index, x, Float);
}

void BlockSetRational(Block* block, Uint index, Rational p)
{
    MEM_SET(block->memory, index, p, Rational);
}

void BlockSetComplex(Block* block, Uint index, Complex z)
{
    MEM_SET(block->memory, index, z, Complex);
}

void BlockSetPtr(Block* block, Uint index, void* ptr)
{
    MEM_SET(block->memory, index, ptr, void *);
}

void BlockGetByteWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, Byte);
}

void BlockGetIntWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, Int);
}

void BlockGetUintWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, Uint);
}

void BlockGetFloatWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, Float);
}

void BlockGetComplexWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, Complex);
}

void BlockGetRationalWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, Rational);
}

void BlockGetPtrWRAP(void* target, const Block* block, Uint index)
{
    TARGET_SET(target, block->memory, index, void *);
}

void BlockSwapByte(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, Byte);
}

void BlockSwapInt(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, Int);
}

void BlockSwapUint(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, Uint);
}

void BlockSwapFloat(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, Float);
}

void BlockSwapRational(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, Rational);
}

void BlockSwapComplex(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, Complex);
}

void BlockSwapPtr(Block* block, Uint j, Uint k)
{
    MEM_SWAP(block->memory, j, k, void *);
}
