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

char* StringNCopy(const char* string, Uint n)
{
    char*   str;

    if (!(str = malloc(n + 1)))
        return NULL;
    
    memcpy(str, string, n);
    str[n] = 0;

    return str;
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

Deck* StringCut(const char* string, Uint cut_size)
{
    Deck*   strings;
    Uint    remaining_length;
    char*   cut;

    if (!(strings = DeckCreatePtr(NULL, MemDestroy)))
        return NULL;
    
    remaining_length = strlen(string);

    while (remaining_length)
    {
        cut_size = cut_size <= remaining_length ? cut_size : remaining_length;

        if (!(cut = StringNCopy(string, cut_size)))
        {
            DeckDestroy(strings);
            return NULL;
        }

        DeckPushBack(strings, cut);
        string += cut_size;
        remaining_length -= cut_size;
    }

    return strings;
}

Deck* StringCutFromEnd(const char* string, Uint cut_size)
{
    Deck*       strings;
    const char* left;
    Uint        length;
    char*       cut;

    if (!(strings = DeckCreatePtr(NULL, MemDestroy)))
        return NULL;
    
    length = strlen(string);
    left = string + length;

    while (length)
    {
        cut_size = length >= cut_size ? cut_size : length;
        left = left - cut_size;
        if (!(cut = StringNCopy(left, cut_size)))
        {
            DeckDestroy(strings);
            return NULL;
        }

        // DeckPushFront(strings, cut);
        DeckPushBack(strings, cut);
        length -= cut_size;
    }

    return strings;
}

char* StringConcat(const char* lhs, const char* rhs)
{
    char* result;
    Uint lhs_length;
    Uint rhs_length;

    lhs_length = strlen(lhs);
    rhs_length = strlen(rhs);

    if (!(result = malloc(lhs_length + rhs_length + 1)))
        return NULL;
    
    memcpy(result, lhs, lhs_length);
    memcpy(result + lhs_length, rhs, rhs_length);

    result[lhs_length + rhs_length] = 0;

    return result;
}

char* StringPrepend(const char* string, char c)
{
    char*   new;
    Uint    length;

    length = strlen(string);
    if (!(new = malloc(length + 2)))
        return NULL;
    
    *new = c;
    memcpy(new + 1, string, length);
    new[length + 1] = 0;

    return new;
}