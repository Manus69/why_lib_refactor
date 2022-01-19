#include "why.h"
#include "declarations.h"
#include "deck.h"

static Int _find_proper_index(const Block* block, Int left_index, Int current, Int (*compare)(const void *, const void *))
{
    while (left_index < current)
    {
        if (BlockCompare(block, left_index, current, compare) <= 0)
            return left_index;

        ++ left_index;
    }

    return left_index;
}

static void _swap_out(Block* block, Int left_index, Int buffer_index)
{
    while (left_index < buffer_index)
    {
        BlockSwap(block, left_index, buffer_index);
        ++ left_index;
    }
}

void InsertionSort(Block* block, Int left_index, Int right_index, Int (*compare)(const void *, const void *))
{
    Int current;
    Int proper_index;

    if (left_index < 0 || right_index < 0)
        return ;

    if (left_index >= right_index)
        return ;
    
    current = left_index + 1;
    while (current <= right_index)
    {
        if (BlockCompare(block, current - 1, current, compare) < 0)
        {
            proper_index = _find_proper_index(block, left_index, current, compare);
            _swap_out(block, proper_index, current);
        }

        ++ current;
    }
}

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

static Int _partition(Block* block, Int left_index, Int right_index, Int (*compare)(const void *, const void *))
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

void QuickSort(Block* block, Int left_index, Int right_index, Int (*compare)(const void *, const void *))
{
    Int pivot_index;

    if (right_index < 0 || left_index < 0)
        return ;

    if (right_index <= left_index)
        return ;

    if (right_index - left_index < Q_SORT_THRESHOLD)
        return InsertionSort(block, left_index, right_index, compare);
    
    pivot_index = _get_pivot_index(left_index, right_index);
    BlockSwap(block, left_index, pivot_index);

    pivot_index = _partition(block, left_index, right_index, compare);

    QuickSort(block, left_index, pivot_index - 1, compare);
    QuickSort(block, pivot_index + 1, right_index, compare);
}

void SortBlock(Block* block, Int (*compare)(const void *, const void *))
{
    Uint n_items;

    if (!block || !compare)
        return ;
    
    n_items = BlockNItems(block);
    if (n_items < 2)
        return ;
    
    return QuickSort(block, 0, n_items - 1, compare);
}

void SortDeck(Deck* deck, Int (*compare)(const void *, const void *))
{
    return QuickSort(deck->block, deck->left_insert_index + 1, deck->right_insert_index - 1, compare);
}