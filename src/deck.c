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

            return deck;
        }
        free(deck);
    }

    return NULL;
}

Deck* DeckCreatePtr(void* (*copy)(const void *), void (*destroy)(void *))
{
    return _create(DECK_CAPACITY, copy, destroy, BlockCreatePtr);
}

static void _destroy_items(Deck* deck)
{
    DeckMap(deck, deck->destroy);
}

void DeckDestroy(Deck* deck)
{
    if (deck->destroy != _destroy_default)
        _destroy_items(deck);
    
    BlockDestroy(deck->block);
    free(deck);
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

void* DeckPointAt(Deck* deck, Uint index)
{
    index = _index_to_abs(deck, index);

    return BlockPointAt(deck->block, index);
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

void DeckMap(Deck* deck, void (*function)(void *))
{
    Uint    n;
    void*   item;

    n = deck->left_insert_index + 1;
    while (n < deck->right_insert_index)
    {
        item = BlockPointAt(deck->block, n);
        function(item);

        ++ n;
    }
}