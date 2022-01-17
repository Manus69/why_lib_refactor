#include "why.h"
#include "declarations.h"

static Int _get_pivot_index(Int left_index, Int right_index)
{
    return left_index + MathRandomInRange(right_index - left_index + 1);
}

static Int _find_first_greater(Block* block, Int pivot_index, Int right_index, Int (*compare)(const void *, const void *))
{
    Int index;

    index = pivot_index + 1;
    while (index <= right_index)
    {
        if (BlockCompare(block, pivot_index, index, compare) > 0)
            return index;
        
        ++ index;
    }

    return NOT_FOUND;
}

static Int _partition(Block* block, Int left_index, Int right_index, Int (*compare)(const void* , const void* ))
{
    Int mid_index;
    Int current_index;

    mid_index = _find_first_greater(block, left_index, right_index, compare);

    if (mid_index == NOT_FOUND)
    {
        BlockSwap(block, left_index, right_index);
        return right_index;
    }

    current_index = mid_index + 1;
    while (current_index <= right_index)
    {
        if (BlockCompare(block, left_index, current_index, compare) < 0)
        {
            BlockSwap(block, mid_index, current_index);
            ++ mid_index;
        }

        ++ current_index;
    }

    -- mid_index;
    BlockSwap(block, left_index, mid_index);

    return mid_index;
}

void QuickSort(Block* block, Int left_index, Int right_index, Int (*compare)(const void* lhs, const void* rhs))
{
    Int pivot_index;

    if (right_index < 0 || left_index < 0)
        return ;

    if (right_index <= left_index)
        return ;
    
    pivot_index = _get_pivot_index(left_index, right_index);
    BlockSwap(block, left_index, pivot_index);

    pivot_index = _partition(block, left_index, right_index, compare);

    QuickSort(block, left_index, pivot_index - 1, compare);
    QuickSort(block, pivot_index + 1, right_index, compare);
}

void Sort(Block* block, Int (*compare)(const void* lhs, const void* rhs))
{
    Uint n_items;

    if (!block || !compare)
        return ;
    
    n_items = BlockGetNItems(block);
    if (n_items < 2)
        return ;
    
    return QuickSort(block, 0, n_items - 1, compare);
}