#include "declarations.h"

static Int _parse_table_row(Table* table, char* string, char col_sep)
{
    Deck*   deck;
    Uint    n;
    Uint    n_items;
    char*   substring;

    if (!(deck = StringSplitDestructive(string, col_sep)))
        return WHY_ERROR;

    n_items = DeckNItems(deck);
    n = 0;

    while (n < n_items)
    {
        substring = *(char **)DeckPointAt(deck, n);
        TablePushLastRow(table, substring);

        ++ n;
    }

    DeckDestroy(deck);

    return TOKEN_PARSE_SUCCESS;
}

static Int _parse_table_rows(Table* table, const Deck* rows, char row_sep_left, char col_sep)
{
    Int     status;
    Uint    n;
    Uint    n_rows;
    char*   row_string;

    n_rows = DeckNItems(rows);
    n = 0;

    while (true)
    {
        row_string = *(char **)DeckPointAt(rows, n);
        while (IsSpace(*row_string))
            ++ row_string;

        if (!*row_string && (n + 1) == n_rows)
            return TOKEN_PARSE_SUCCESS;

        if (row_sep_left && *row_string != row_sep_left)
            return 0;
        else if (row_sep_left)
            ++ row_string;

        if ((status = _parse_table_row(table, row_string, col_sep)) <= 0)
            return status;

        ++ n;

        if (n == n_rows)
            break ;

        TableAddRow(table);
    }

    return TOKEN_PARSE_SUCCESS;
}

static void _check_separators(char* row_sep_right, char* col_sep)
{
    if (!*row_sep_right)
        *row_sep_right = '\n';
    
    if (!*col_sep)
        *col_sep = ' ';
}

Int ParseTable(Table* table, const char* string, char table_sep_left, char table_sep_right,
                char row_sep_left, char row_sep_right, char col_sep)
{
    Int     status;
    Int     right_index;
    Deck*   rows;
    char*   copy;

    if (!table_sep_right)
        right_index = strlen(string) - 1;
    else
        right_index = StringFindC(string, table_sep_right);
    
    if (right_index == NOT_FOUND)
        return 0;

    while (IsSpace(*string))
        ++ string;

    if (table_sep_left && *string != table_sep_left)
        return 0;
    else if (table_sep_left)
    {
        ++ string;
        -- right_index;
    }

    if (!(copy = StringSubstring(string, right_index + 1)))
        return WHY_ERROR;
    
    if (table_sep_right)
        copy[right_index] = '\0';

    _check_separators(&row_sep_right, &col_sep);

    if (!(rows = StringSplitLengthDestructive(copy, row_sep_right, right_index + 1)))
    {
        free(copy);
        return 0;
    }
    
    WhySavePtr(copy);
    status = _parse_table_rows(table, rows, row_sep_left, col_sep);

    DeckDestroy(rows); 

    return status <= 0 ? status : right_index + 1;
}