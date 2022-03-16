#include "deck.h"
#include "block.h"
#include "declarations.h"

static Uint _index_to_abs(const Deck* deck, Uint index)
{
    return deck->left_insert_index + index + 1;
}

static void* _copy_default(const void* ptr)
{
    return (void *)ptr;
}

static void _destroy_default(void* ptr)
{
    (void)ptr;
}

static Deck* _create(Uint n_items, void* (*copy)(const void *), void (*destroy)(void *), Block* (*block_create)(Uint))
{
    Deck* deck;

    if ((deck = malloc(sizeof(*deck))))
    {
        if ((deck->block = block_create(n_items)))
        {
            deck->copy = copy ? copy : _copy_default;
            deck->destroy = destroy ? destroy : _destroy_default;
            deck->left_insert_index = 0;
            deck->right_insert_index = deck->left_insert_index + 1;
            deck->block_constructor = block_create;

            return deck;
        }
        free(deck);
    }

    return NULL;
}

Deck* DeckCreateInherit(const Deck* deck)
{
    Deck* new_deck;

    if (!(new_deck = _create(DeckNItems(deck), deck->copy, deck->destroy, deck->block_constructor)))
        return NULL;
    
    return new_deck;
}

Deck* DeckCreatePtr(void* (*copy)(const void *), void (*destroy)(void *))
{
    return _create(DECK_CAPACITY, copy, destroy, BlockCreatePtr);
}

Deck* DeckCreatePtrCapacity(void* (*copy)(const void *), void (*destroy)(void *), Uint capacity)
{
    return _create(capacity, copy, destroy, BlockCreatePtr);
}

Deck* DeckCreateUint()
{
    return _create(DECK_CAPACITY, NULL, NULL, BlockCreateUint);
}

Deck* DeckCreateUintCapacity(Uint capacity)
{
    return _create(capacity, NULL, NULL, BlockCreateUint);
}

static void _destroy_items(Deck* deck)
{
    DeckMap(deck, deck->destroy);
}

void DeckDestroy(Deck* deck)
{
    if (!deck)
        return ;
    
    if (deck->destroy != _destroy_default)
        _destroy_items(deck);
    
    BlockDestroy(deck->block);
    free(deck);
}

void DeckDestroyWRAP(void* deck)
{
    DeckDestroy(*(Deck **)deck);
}

Deck* DeckCopyStructure(const Deck* deck)
{
    Deck* new_deck;

    if (!(new_deck = _create(DECK_CAPACITY, deck->copy, deck->destroy, deck->block_constructor)))
        return NULL;
    
    return new_deck;
}

Uint DeckNItems(const Deck* deck)
{
    return deck->right_insert_index - deck->left_insert_index - 1;
}

void DeckSet(Deck* deck, Uint index, const void* item)
{
    index = _index_to_abs(deck, index);
    BlockSet(deck->block, index, item);
}

void* DeckPointAt(const Deck* deck, Uint index)
{
    index = _index_to_abs(deck, index);

    return BlockPointAt(deck->block, index);
}

void DeckGet(void* target, const Deck* deck, Uint index)
{
    index = _index_to_abs(deck, index);

    return BlockGet(target, deck->block, index);
}

Int DeckPushBack(Deck* deck, const void* item)
{
    if (deck->right_insert_index == deck->block->n_items)
    {
        if (BlockExpand(deck->block, deck->block->n_items) != WHY_OK)
            return WHY_ERROR;
    }

    BlockSet(deck->block, deck->right_insert_index, item);
    ++ deck->right_insert_index;

    return WHY_OK;
}

static Uint _move_items(Deck* deck)
{
    Uint    n_items;
    Uint    left_index;
    void*   ptr;

    n_items = DeckNItems(deck);
    left_index = deck->block->n_items / 4;
    ptr = BlockPointAt(deck->block, left_index);
    memmove(ptr, deck->block->memory, n_items);
    deck->left_insert_index = left_index;
    deck->right_insert_index = left_index + n_items + 1;

    return left_index;
}

Int DeckPushFront(Deck* deck, const void* item)
{
    Uint left_index;

    left_index = deck->left_insert_index;
    if (deck->left_insert_index == 0)
    {
        if (BlockExpand(deck->block, deck->block->n_items) != WHY_OK)
            return WHY_ERROR;

        left_index = _move_items(deck);
    }

    BlockSet(deck->block, left_index, item);
    -- deck->left_insert_index;

    return WHY_OK;
}

void DeckReserve(Deck* deck, Uint n_items)
{
    if (deck->block->n_items < n_items)
        BlockExpand(deck->block, n_items - deck->block->n_items);
}

Int DeckLast(void* target, const Deck* deck)
{
    if (DeckNItems(deck))
    {
        BlockGet(target, deck->block, deck->right_insert_index - 1);

        return WHY_OK;
    }
    
    return WHY_ERROR;
}

Int DeckFirst(void* target, const Deck* deck)
{
    if (DeckNItems(deck))
    {
        BlockGet(target, deck->block, deck->left_insert_index + 1);

        return WHY_OK;
    }

    return WHY_ERROR;
}

void DeckMapRange(Deck* deck, void (*function)(void *), Uint left_index, Uint n_items)
{
    void* item;

    left_index = _index_to_abs(deck, left_index);
    while (n_items)
    {
        item = BlockPointAt(deck->block, left_index);
        function(item);

        ++ left_index;
        -- n_items;
    }
}

void DeckMap(Deck* deck, void (*function)(void *))
{
    DeckMapRange(deck, function, 0, DeckNItems(deck));
}

void DeckAppend(Deck* lhs, const Deck* rhs)
{
    Uint    n;
    void*   item;

    n = 0;
    while (n < DeckNItems(rhs))
    {
        item = DeckPointAt(rhs, n);
        DeckPushBack(lhs, item);

        ++ n;
    }
}

void DeckReverse(Deck* deck)
{
    if (DeckNItems(deck) == 0)
        return ;
    
    BlockReverseSlice(deck->block, deck->left_insert_index + 1, deck->right_insert_index - 1);
}

void* DeckSearchLinear(const Deck* deck, const void* item, Int (*compare)(const void *, const void *))
{
    if (DeckNItems(deck) == 0)
        return NULL;

    return BlockSearchRange(deck->block, item, compare, deck->left_insert_index + 1, deck->right_insert_index - 1);
}

void* DeckBinSearch(const Deck* deck, const void* item, Int (*compare)(const void *, const void *))
{
    if (DeckNItems(deck) == 0)
        return NULL;
    
    return BlockBinSearchRange(deck->block, item, compare, deck->left_insert_index + 1, deck->right_insert_index - 1);
}

Int DeckCompare(const Deck* deck, Uint j, Uint k, Int (*compare)(const void *, const void *))
{
    void* lhs;
    void* rhs;

    lhs = DeckPointAt(deck, j);
    rhs = DeckPointAt(deck, k);

    return compare(lhs, rhs);
}

Deck* DeckUnique(Deck* deck, Int (*compare)(const void *, const void *))
{
    Deck*   result;
    void*   item;
    Uint    n;

    SortDeck(deck, compare);
    
    if (!(result = _create(DeckNItems(deck), NULL, NULL, deck->block_constructor)))
        return NULL;

    if (DeckNItems(deck) == 0)
        return result;

    item = DeckPointAt(deck, 0);
    DeckPushBack(deck, item);
    n = 1;

    while (n < DeckNItems(deck))
    {
        if (DeckCompare(deck, n - 1, n, compare) != 0)
        {
            item = DeckPointAt(deck, n);
            DeckPushBack(result, item);
        }

        ++ n;
    }

    return result;
}

Deck* DeckFilter(const Deck* deck, bool (*predicate)(const void *))
{
    Deck*   new_deck;
    void*   item;
    Uint    n;
    Uint    n_items;

    n_items = DeckNItems(deck);
    if (!(new_deck =_create(n_items, NULL, NULL, deck->block_constructor)))
        return NULL;

    n = 0;
    while (n < n_items)
    {
        item = DeckPointAt(deck, n);

        if (predicate(item))
            DeckPushBack(new_deck, item);
        
        ++ n;
    }

    return new_deck;
}

void DeckFold(void* target, const Deck* deck, void (*fold)(void *, const void *, const void *))
{
    Uint n_items;

    n_items = DeckNItems(deck);
    if (n_items == 0)
        return ;
    
    BlockFoldNItems(target, deck->block, deck->left_insert_index + 1, n_items, fold);
}

void SortDeckSlice(Deck* deck, Uint left, Uint right, Int (*compare)(const void *, const void *))
{
    return QuickSort(deck->block, _index_to_abs(deck, left), _index_to_abs(deck, right), compare);
}