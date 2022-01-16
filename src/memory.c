#include "why.h"
#include "macro.h"

#include <stdlib.h>
#include <string.h>

void* mem_expand(void* memory, Uint size, Uint extra_size)
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
