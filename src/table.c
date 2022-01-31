#include "table.h"
#include "deck.h"
#include "declarations.h"

Int TableAddRow(Table* table)
{
    Deck* first_row;
    Deck* row;

    DeckGet(&first_row, table->rows, 0);
    row = DeckCreatePtr(first_row->copy, first_row->destroy);

    return DeckPushBack(table->rows, row);
}

Table* TableCreatePtr(void* (*copy)(const void *), void (*destroy)(void *))
{
    Table*  table;
    Deck*   row;

    if (!(table = malloc(sizeof(*table))))
        return NULL;
    
    if (!(table->rows = DeckCreatePtr(NULL, DeckDestroyWRAP)))
    {
        free(table);
        return NULL;
    }

    if (!(row = DeckCreatePtr(copy, destroy)))
    {
        free(table->rows);
        free(table);

        return NULL;
    }
    
    DeckPushBack(table->rows, row);

    return table;
}

Uint TableNRows(const Table* table)
{
    return DeckNItems(table->rows);
}

Int TablePush(Table* table, Uint row, const void* item)
{
    Deck* deck;

    DeckGet(&deck, table->rows, row);

    return DeckPushBack(deck, item);
}

Int TablePushLastRow(Table* table, const void* item)
{
    return TablePush(table, DeckNItems(table->rows) - 1, item);
}

void TableMapRow(Table* table, Uint n, void (*function)(void *))
{
    Deck* row;

    DeckGet(&row, table->rows, n);
    DeckMap(row, function);
}

void TableMap(Table* table, void (*function)(void *))
{
    Uint    n_rows;
    Uint    n;
    Deck*   row;

    n_rows = DeckNItems(table->rows);
    n = 0;

    while (n < n_rows)
    {
        DeckGet(&row, table->rows, n);
        DeckMap(row, function);

        ++ n;
    }
}

void TableDestroy(Table* table)
{
    Uint    n;
    Uint    n_items;
    Deck*   row;

    if (!table)
        return ;

    n_items = DeckNItems(table->rows);
    n = 0;

    while (n < n_items)
    {
        DeckGet(&row, table->rows, n);
        DeckDestroy(row);

        ++ n;
    }

    BlockDestroy(table->rows->block);
    free(table->rows);
    free(table);
}

void TableDestroyWRAP(void* table_pointer)
{
    TableDestroy(*(Table **)table_pointer);
}

Int TableLast(void* target, const Table* table)
{
    Deck* row;

    if ((DeckLast(&row, table->rows)) != WHY_ERROR)
        return DeckLast(target, row);

    return WHY_ERROR;
}

Int TableFirst(void* target, const Table* table)
{
    Deck* row;

    if ((DeckFirst(&row, table->rows)) != WHY_ERROR)
        return DeckFirst(target, table->rows);

    return WHY_ERROR;
}

void* TablePointAt(const Table* table, Uint row, Uint col)
{
    Deck* _row;

    _row = *(Deck **)DeckPointAt(table->rows, row);

    return DeckPointAt(_row, col);
}

Deck* TablePointAtRow(const Table* table, Uint row)
{
    return *(Deck **)DeckPointAt(table->rows, row);
}