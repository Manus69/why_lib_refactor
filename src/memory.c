#include "why.h"
#include "macro.h"
#include "declarations.h"

void* MemExpand(void* memory, Uint size, Uint extra_size)
{
    void*   new_memory;
    Uint    new_size;
    
    new_size = size + extra_size;

    if (new_size < size)
        return NULL;

    if (!memory)
        return malloc(new_size);

    if ((new_memory = malloc(new_size)))
        return memcpy(new_memory, memory, size);

    return NULL;
}

void* MemExpandZero(void* memory, Uint size, Uint extra_size)
{
    void* new;

    if (!(new = MemExpand(memory, size, extra_size)))
        return NULL;
    
    memset(new + size, 0, extra_size);

    return new;
}


void MemDestroy(void* ptr)
{
    free(*(void **)ptr);
}