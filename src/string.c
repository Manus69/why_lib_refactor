#include "declarations.h"

char* StringSubstring(const char* string, Uint length)
{
    return strndup(string, length);
}

Int StringFindC(const char* string, char c)
{
    Uint n;

    n = 0;
    while (string[n])
    {
        if (string[n] == c)
            return n;

        ++ n;
    }

    return NOT_FOUND;
}

Deck* StringSplitLengthDestructive(char* string, char separator, Uint length)
{
    Deck* deck;
    char* current;
    char* last;

    if (!(deck = DeckCreatePtr(NULL, NULL)))
        return NULL;

    current = string;
    last = string + length;

    while (true)
    {
        DeckPushBack(deck, current);

        while (*current != separator && current < last)
            ++ current;

        if (current == last)
            break ;
        
        *current = '\0';
        ++ current;
    }

    return deck;
}

Deck* StringSplitLength(const char* string, char separator, Uint length)
{
    char* copy;

    if (!(copy = StringSubstring(string, length)))
        return NULL;
    
    WhySavePtr(copy);
    
    return StringSplitLengthDestructive(copy, separator, length);
}

Deck* StringSplit(const char* string, char separator)
{
    Uint length;

    length = strlen(string);

    return StringSplitLength(string, separator, length);
}

Deck* StringSplitDestructive(char* string, char separator)
{
    Uint length;

    length = strlen(string);

    return StringSplitLengthDestructive(string, separator, length);
}

Deck* StringSplitStr(char* string, const char* substring)
{
    Deck*   strings;
    Uint    length;
    char*   current;
    char*   next;

    if (!(strings = DeckCreatePtr(NULL, NULL)))
        return NULL;
    
    length = strlen(substring);
    current = string;

    while (true)
    {
        DeckPushBack(strings, current);
        next = strstr(current, substring);

        if (!next)
            return strings;

        memset(next, 0, length);
        current = next + length;
    }
}

Byte* StringSplice(const Deck* strings)
{
    Block*  block;
    Uint    index;
    Uint    n;
    Uint    length;
    Uint    n_items;
    Byte*   string;

    if (!(block = BlockCreateByte(BLOCK_CAPACITY)))
        return NULL;
    
    n = 0;
    index = 0;
    n_items = DeckNItems(strings);

    while (n < n_items)
    {
        DeckGet(&string, strings, n);

        length = strlen((char *)string);
        if (BlockWriteBytes(block, index, string, length) != WHY_OK)
        {
            BlockDestroy(block);
            return NULL;
        }

        index += length;
        ++ n;
    }

    BlockSet(block, index, &ZERO_BYTE);

    return BlockDestroyReturnContent(block);
}

Byte* StringSplitSplice(char* string, const char* substring)
{
    Deck* strings;
    Byte* new_string;

    if (!(strings = StringSplitStr(string, substring)))
        return NULL;

    new_string = StringSplice(strings);
    DeckDestroy(strings);

    return new_string;
}