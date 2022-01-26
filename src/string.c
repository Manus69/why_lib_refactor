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
