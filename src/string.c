#include "declarations.h"
#include "macro.h"
#include "block.h"

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

Int StringFindCLast(const char* string, char c)
{
    Int n;

    n = strlen(string) - 1;

    while (n >= 0)
    {
        if (string[n] == c)
            return n;
        -- n;
    }

    return NOT_FOUND;
}

bool StringStartsWith(const char* string, const char* start)
{
    Uint length;

    length = strlen(start);

    return strncmp(string, start, length) == 0;
}

bool StringEndsWith(const char* string, const char* suffix)
{
    Uint suffix_length;
    Uint lhs_length;

    suffix_length = strlen(suffix);
    lhs_length = strlen(string);

    if (lhs_length < suffix_length)
        return false;
    
    string += (lhs_length - suffix_length);

    return strncmp(string, suffix, suffix_length) == 0;
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
        DeckPushBack(deck, &current);

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
    
    WhySavePtr(&copy);
    
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
        DeckPushBack(strings, &current);
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

        DeckPushBack(strings, &cut);
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
        DeckPushBack(strings, &cut);
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

char* StringJoin(const char* lhs, const char* mid, const char* rhs)
{
    Uint    lhs_length, mid_length, rhs_length;
    char*   result;

    lhs_length = strlen(lhs);
    mid_length = strlen(mid);
    rhs_length = strlen(rhs);

    if (!(result = malloc(lhs_length + mid_length + rhs_length + 1)))
        return NULL;
    
    memcpy(result, lhs, lhs_length);
    memcpy(result + lhs_length, mid, mid_length);
    memcpy(result + lhs_length + mid_length, rhs, rhs_length);

    result[lhs_length + mid_length + rhs_length] = 0;

    return result;
}

char* StringToLowerDestructive(char* string)
{
    char*   ptr;
    char    diff;

    diff = 'a' - 'A';
    ptr = string;
    while (*ptr)
    {
        if (IsUpper(*ptr))
            *ptr = *ptr + diff;
        
        ++ ptr;
    }

    return string;
}

char* StringToLower(const char* string)
{
    char* copy;

    copy = strdup(string);

    return StringToLowerDestructive(copy);
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

char* StringEnclose(const char* string, const char* lhs, const char* rhs)
{
    char*   result;
    Uint    length;
    Uint    old_length;
    Uint    lhs_length;
    Uint    rhs_length;

    old_length = strlen(string);
    lhs_length = strlen(lhs);
    rhs_length = strlen(rhs);

    length = old_length + lhs_length + rhs_length;
    if (!(result = malloc(length + 1)))
        return NULL;
    
    memcpy(result, lhs, lhs_length);
    memcpy(result + lhs_length, string, old_length);
    memcpy(result + lhs_length + old_length, rhs, rhs_length);

    result[length] = '\0';

    return result;
}

char* StringStripFront(const char* string, char c)
{
    Uint    length;

    while (*string && (IsSpace(*string) || (*string == c)))
        ++ string;
    
    length = strlen(string);

    return StringNCopy(string, length);
}

void StringStripBackDestructive(char* string, char c)
{
    while (*string && (!IsSpace(*string) && *string != c))
        ++ string;
    
    *string = 0;
}

char* StringStrip(const char* string, char front, char back)
{
    char* result;

    if (!(result = StringStripFront(string, front)))
        return NULL;

    StringStripBackDestructive(result, back);

    return result;
}

void StringReverseLength(char* string, Uint length)
{
    Uint lhs;
    Uint rhs;

    if (length < 2)
        return ;

    lhs = 0;
    rhs = length - 1;

    while (lhs < rhs)
    {
        SWAP(string[lhs], string[rhs]);
        ++ lhs;
        -- rhs;
    }
}

void StringReverse(char* string)
{
    Uint length;

    length = strlen(string);

    if (length < 2)
        return ;

    StringReverseLength(string, length);
}

char* StringCreateReversed(const char* string)
{
    char* copy;

    copy = strdup(string);
    StringReverse(copy);

    return copy;
}

Uint StringHashLength(const char* string, Uint length)
{
    Uint value;

    value = HASH_VALUE;
    while (length)
    {
        value = ((value << 5) + value) + *string;
        ++ string;
        -- length;
    }

    return value;
}

Uint StringHash(const char* string)
{
    Uint value;

    value = HASH_VALUE;
    while (*string)
    {
        value = ((value << 5) + value) + *string;
        ++ string;
    }

    return value;
}

Uint StringHashWRAP(const void* string)
{
    return StringHash(*(char **)string);
}

bool StringPermuteLexicalSlice(char* string, Uint length)
{
    Block block;

    block.interface = &ByteInterface;
    block.memory = string;
    block.n_items = length;

    return BlockPermuteLexicalSlice(&block, 0, block.n_items - 1, CompareChar);
}

bool StringPermuteLexical(char* string)
{
    Uint length;

    length = strlen(string);

    return StringPermuteLexicalSlice(string, length);
}