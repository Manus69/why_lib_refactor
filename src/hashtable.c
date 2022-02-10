#include "hashtable.h"

#define CELL_CAPACITY (2)

static void _destroy_cells(Block* cells, Uint n_cells)
{
    BlockMap(cells, 0, n_cells, DeckDestroyWRAP);
}

static Int _init_cells(Block* cells, void* (*copy)(const void *), void (*destroy)(void *))
{
    Uint    n;
    Uint    n_intems;
    Deck*   cell;

    n = 0;
    n_intems = BlockNItems(cells);

    while (n < n_intems)
    {
        if (!(cell = DeckCreatePtrCapacity(copy, destroy, CELL_CAPACITY)))
        {
            _destroy_cells(cells, n);
            return WHY_ERROR;
        }
        
        BlockSet(cells, n, &cell);
        ++ n;
    }

    return WHY_OK;
}

HashTable* HashTableCreate(Uint capacity, void* (*copy)(const void *),
                            void (*destroy)(void *), Uint (*hash)(const void *))
{
    HashTable* table;

    if ((table = malloc(sizeof(*table))))
    {
        if ((table->table = BlockCreatePtr(capacity)))
        {
            if (_init_cells(table->table, copy, destroy) == WHY_OK)
            {
                table->n_items = 0;
                table->hash_function = hash;

                return table;
            }
            BlockDestroy(table->table);
        }
        free(table);
    }

    return NULL;
}

Uint HashTableNItems(const HashTable* table)
{
    return table->n_items;
}

Uint HashTableNCells(const HashTable* table)
{
    return BlockNItems(table->table);
}

void HashTableDestroy(HashTable* table)
{
    Uint n_items;

    if (!table)
        return ;
    
    n_items = BlockNItems(table->table);
    _destroy_cells(table->table, n_items);
    BlockDestroy(table->table);
    free(table);
}

void HashTableMapCell(HashTable* table, Uint n, void (*function)(void *))
{
    Deck*   cell;

    cell = *(Deck **)BlockPointAt(table->table, n);
    DeckMap(cell, function);
}

static Int _insert(HashTable* table, const void* item, Uint index)
{
    Deck* cell;

    cell = *(Deck **)BlockPointAt(table->table, index);

    return DeckPushBack(cell, item);
}

Int HashTableInsert(HashTable* table, const void* item)
{
    Uint hash;
    Uint index;

    hash = table->hash_function(item);
    index = hash % HashTableNCells(table);

    return _insert(table, item, index);
}

HashTable* HashDeck(const Deck* deck, Uint capacity, Uint (*hash)(const void *),
                    void* (*copy)(const void *), void (*destroy)(void *))
{
    HashTable*  table;
    Uint        n;
    void*       item;

    if (!(table = HashTableCreate(capacity, copy, destroy, hash)))
        return NULL;
    
    n = 0;
    while (n < DeckNItems(deck))
    {
        item = DeckPointAt(deck, n);
        HashTableInsert(table, item);

        ++ n;
    }

    return table;
}