#include "declarations.h"
#include "block.h"

#define BLOCK_MIN_SIZE (1)

static Block* _create(Uint n_items, const TypeInterface* interface)
{
    Block* block;

    n_items = (n_items < BLOCK_MIN_SIZE) ? BLOCK_MIN_SIZE : n_items;

    if ((block = malloc(sizeof(*block))))
    {
        if ((block->memory = malloc(n_items * interface->size)))
        {
            block->n_items = n_items;
            block->interface = interface;

            return block;
        }
        free(block);
    }

    return NULL;
}

Block* BlockCreateByte(Uint n_items)
{
    return _create(n_items, &ByteInterface);
}

Block* BlockCreateInt(Uint n_items)
{
    return _create(n_items, &IntInterface);
}

Block* BlockCreateUint(Uint n_items)
{
    return _create(n_items, &UintInterface);
}

Block* BlockCreateFloat(Uint n_items)
{
    return _create(n_items, &FloatInterface);
}

Block* BlockCreateRational(Uint n_items)
{
    return _create(n_items, &RationalInterface);
}

Block* BlockCreateComplex(Uint n_items)
{
    return _create(n_items, &ComplexInterface);
}

Block* BlockCreatePtr(Uint n_items)
{
    return _create(n_items, &PtrInterface);
}

void BlockInitFromArray(Block* block, const void* array)
{
    memcpy(block->memory, array, BlockGetSize(block));
}

void BlockDestroyNoMem(Block* block)
{
    if (!block)
        return ;
    
    free(block);
}

void BlockDestroy(Block* block)
{
    if (!block)
        return ;
    
    free(block->memory);
    free(block);
}

void BlockDestroyWRAP(void* block_pointer)
{
    BlockDestroy(*(Block **)block_pointer);
}

void* BlockDestroyReturnContent(Block* block)
{
    void* content;

    if (!block)
        return NULL;
    
    content = block->memory;
    free(block);

    return content;
}

Uint BlockGetSize(const Block* block)
{
    return block->interface->size * block->n_items;
}

void* BlockGetContent(const Block* block)
{
    return block->memory;
}

Uint BlockNItems(const Block* block)
{
    return block->n_items;
}

Int BlockExpand(Block* block, Uint extra_items)
{
    Uint    extra_size;
    void*   new_memory;

    extra_size = extra_items * block->interface->size;
    new_memory = realloc(block->memory, BlockGetSize(block) + extra_size);

    if (!new_memory)
        return WHY_ERROR;

    block->memory = new_memory;
    block->n_items += extra_items;

    return WHY_OK;
}

void* BlockPointAt(const Block* block, Uint index)
{
    return block->memory + (index * block->interface->size);
}

Int BlockCopyItems(Block* block, Uint index, const void* pointer, Uint n_items)
{
    void* ptr;

    if (index + n_items > block->n_items)
    {
        if (BlockExpand(block, n_items) != WHY_OK)
            return WHY_ERROR;
    }

    ptr = BlockPointAt(block, index);
    memcpy(ptr, pointer, n_items * block->interface->size);

    return WHY_OK;
}

ssize_t BlockReadFromFile(Block* block, Uint index, int file, Uint n_bytes)
{
    Uint extra_items;

    assert(block->interface == &ByteInterface);

    if (index + n_bytes > block->n_items)
    {
        extra_items = n_bytes > block->n_items ? n_bytes : block->n_items;
        if (BlockExpand(block, extra_items) != WHY_OK)
            return -1;
    }

    return read(file, block->memory + index, n_bytes);
}

void BlockGet(void* target, const Block* block, Uint index)
{
    return block->interface->get(target, block->memory, index);
}

void BlockSet(Block* block, Uint index, const void* item)
{
    return block->interface->set(block->memory, index, item);
}

void BlockSwap(Block* block, Uint j, Uint k)
{
    return block->interface->swap(block->memory, j, k);
}

Int BlockCompare(const Block* block, Uint j, Uint k, Int (*compare)(const void* lhs, const void* rhs))
{
    void* _lhs;
    void* _rhs;

    _lhs = BlockPointAt(block, j);
    _rhs = BlockPointAt(block, k);

    return compare(_lhs, _rhs);
}

void BlockMap(const Block* block, Uint index, Uint n_items, void (*function)(const void *))
{
    void*   item;
    Uint    cap;

    cap = index + n_items;
    while (index < cap)
    {
        item = BlockPointAt(block, index);
        function(item);

        ++ index;
    }
}

void BlockFoldNItems(void* target, const Block* block, Uint index, Uint n_items, 
                    void (*fold)(void *, const void *, const void *))
{
    Uint    last_index;
    void*   item;

    last_index = index + n_items;
    while (index < last_index)
    {
        item = BlockPointAt(block, index);
        fold(target, target, item);

        ++ index;
    }
}

void BlockFold(void* target, const Block* block, void (*fold)(void *, const void *, const void *))
{
    return BlockFoldNItems(target, block, 0, BlockNItems(block), fold);
}

Int BlockWriteBytes(Block* block, Uint index, const Byte* bytes, Uint n_bytes)
{
    if (index + n_bytes + 1 > BlockGetSize(block))
    {
        if (BlockExpand(block, n_bytes + 1) != WHY_OK)
            return WHY_ERROR;
    }

    memcpy(block->memory + index, bytes, n_bytes);

    return WHY_OK;
}

static bool _check_left(const Block* block, const void* item, Int (*compare)(const void *, const void *))
{
    const void* _item;

    _item = BlockPointAt(block, 0);

    if (compare(_item, item) < 0)
        return false;
    
    return true;
}

static bool _check_right(const Block* block, const void* item, Int (*compare)(const void *, const void *))
{
    const void* _item;

    _item = BlockPointAt(block, block->n_items - 1);

    if (compare(_item, item) > 0)
        return false;
    
    return true;
}

void* BlockBinSearch(const Block* block, const void* item, Int (*compare)(const void *, const void *))
{
    Uint    left;
    Uint    right;
    Uint    index;
    Int     result;
    void*   _item;

    if (BlockNItems(block) == 0)
        return NULL;
    
    if (!_check_left(block, item, compare))
        return NULL;
    
    if (!_check_right(block, item, compare))
        return NULL;

    left = 0;
    right = BlockNItems(block) - 1;
    
    while (left < right)
    {
        index = (left + right) / 2;

        _item = BlockPointAt(block, index);
        result = compare(_item, item);

        if (result == 0)
            return _item;
        else if (result > 0)
            left = index + 1;
        else
            right = index - 1;
    }

    return NULL;
}

void BlockReverseSlice(Block* block, Uint left, Uint right)
{
    while (left < right)
    {
        BlockSwap(block, left, right);
        ++ left;
        -- right;
    }
}

void BlockReverse(Block* block)
{
    if (block->n_items == 0)
        return ;
    
    BlockReverseSlice(block, 0, block->n_items - 1);
}