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

Deck* ReadFileAllLines(const char* name)
{
    Byte* bytes;
    Deck* lines;
    Byte* current;

    if (!(bytes = ReadFile(name)))
        return NULL;

    WhySavePtr(bytes);

    if (!(lines = DeckCreatePtr(NULL, NULL)))
        return NULL;

    DeckReserve(lines, (1 << 12));
    current = bytes;
    while (true)
    {
        while (*current && *current == '\n')
            ++ current;
        
        if (!*current)
            break ;

        DeckPushBack(lines, &current);

        while (*current && *current != '\n')
            ++ current;
        
        if (!*current)
            break ;
        
        *current = '\0';
        ++ current;
    }

    return lines;
}

Deck* ReadFileAllLines2(const char* name)
{
    Deck* lines;
    Byte* bytes;

    if (!(bytes = ReadFile(name)))
        return NULL;

    WhySavePtr(bytes);

    lines = StringSplitDestructive((char *)bytes, '\n');

    return lines;
}

Byte* ReadFileSplitSplice(const char* name, const char* substring)
{
    Byte* bytes;
    Byte* output;

    if (!(bytes = ReadFile(name)))
        return NULL;
    
    if (!(output = StringSplitSplice((char *)bytes, substring)))
        return NULL;
    
    free(bytes);

    return output;
}