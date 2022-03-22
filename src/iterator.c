#include "declarations.h"

void IteratorInit(Iterator* iterator)
{
    iterator->index = 0;
    iterator->item_pointer = NULL;
}

bool IteratorDone(Iterator* iterator)
{
    return iterator->index < 0;
}