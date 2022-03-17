#include "declarations.h"

Int FileWriteStringInto(Int file, const char* string)
{
    Uint length;

    length = strlen(string);
    if (write(file, string, length) < 0)
        return WHY_ERROR;
    
    return WHY_OK;
}

Int FileWriteStringIntoN(Int file, const char* string)
{
    Uint    length;
    char*   new_line;

    length = strlen(string);
    new_line = "\n";

    if (write(file, string, length) < 0)
        return WHY_ERROR;

    if (write(file, new_line, 1) < 0)
        return WHY_ERROR;

    return WHY_OK;
}

Int FileWriteStringsInto(Int file, const Deck* strings)
{
    char*   string;
    Uint    n;
    Uint    n_items;

    n = 0;
    n_items = DeckNItems(strings);
    while (n < n_items)
    {
        string = *(char **)DeckPointAt(strings, n);
        FileWriteStringIntoN(file, string);

        ++ n;
    }

    return WHY_OK;
}