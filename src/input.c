#include "why.h"
#include "declarations.h"
#include "block.h"

#define READ_SIZE (1 << 12)

static void* _read_abort(Block* block, int file)
{
    BlockDestroy(block);
    close(file);

    return NULL;
}

Byte* ReadFile(const char* name)
{
    int     file;
    ssize_t read_size;
    Uint    index;
    Block*  block;
    Byte    zero;

    if ((file = open(name, O_RDONLY)) < 0)
        return NULL;

    block = BlockCreateByte(READ_SIZE);
    index = 0;
    zero = 0;

    while ((read_size = BlockReadFromFile(block, index, file, READ_SIZE)))
    {
        if (read_size < 0)
            return _read_abort(block, file);
        
        index += read_size;
    }

    if (index == block->n_items && (BlockExpand(block, 1) != WHY_OK))
        return _read_abort(block, file);
    
    BlockSet(block, index, &zero);
    close(file);

    return BlockDestroyReturnContent(block);
}

Array* ReadFileByLine(const char* name)
{
    (void)name;

    return NULL;
}