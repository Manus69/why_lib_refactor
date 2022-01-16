#include "why.h"
#include "declarations.h"
#include "block.h"

#define DEFAULT_READ_SIZE (1 << 10)

static void* _read_abort(Block* block, int file)
{
    free(block->memory);
    close(file);

    return NULL;
}

Byte* ReadFile(const char* name)
{
    int     file;
    Uint    index;
    struct Block   block;
    ssize_t read_size;

    //
    Byte* buffer;
    //

    // block = (Block){.n_items = 0, };
    if ((file = open(name, O_RDONLY)) < 0)
        return NULL;
    
    BlockInitByte(&block, DEFAULT_READ_SIZE);
    index = 0;

    while (true)
    {
        buffer = block.memory + index;
        read_size = read(file, buffer, DEFAULT_READ_SIZE);
        if (read_size < 0)
            return _read_abort(&block, file);
        else if (read_size == 0)
            break ;
        
        index += read_size;
        
        if (index >= block.n_items && (BlockExpand(&block, DEFAULT_READ_SIZE) != WHY_OK))
            return _read_abort(&block, file);
    }

    if (index >= block.n_items && (BlockExpand(&block, 1) != WHY_OK))
        return _read_abort(&block, file);
    
    BlockSetByte(&block, index, 0);
    close(file);

    return block.memory;
}

Block* ReadFileByLine(const char* name)
{
    (void)name;

    return NULL;
}