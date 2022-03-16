#include "why.h"
#include "macro.h"
#include "rational.h"
#include "natural.h"
#include "declarations.h"

#define NEW_LINE    ("\n")
#define SPACE       (" ")
#define NULL_MSG    ("NULL")

void PrintObject(const void* object, void (*print)(const void *, const void *), const void* sep)
{
    print(object, sep);
}

void PrintCstr(const char* str)
{
    if (str)
        printf("%s", str);
}

void PrintCstrWRAP(const void* str, const void* sep)
{
    PrintCstr(*(char **)str);
    PrintCstr(sep);
}

void PrintCstrN(const void* str)
{
    PrintCstrWRAP(str, NEW_LINE);
}

void PrintCstrS(const void* str)
{
    PrintCstrWRAP(str, SPACE);
}

void PrintCstrP(const void* str)
{
    printf("\"");
    PrintCstr(*(char **)str);
    printf("\" ");
}

void PrintRational(const Rational* p)
{
    Int top;
    Int bot;

    top = p->top;
    bot = p->bot;

    if (bot == 1)
        printf("%ld", top);
    else if (bot == -1)
        printf("%ld", -top);
    else
        printf("%ld/%ld", p->top, p->bot);
}

void PrintRationalWRAP(const void* p, const void* sep)
{
    PrintRational(*(Rational **)p);
    PrintCstr(sep);
}

void PrintRationalN(const void* p)
{
    PrintRationalWRAP(p, NEW_LINE);
}

void PrintRationalS(const void* p)
{
    PrintRationalWRAP(p, SPACE);
}

void PrintRationalP(const void* p)
{
    printf("(");
    PrintRational(p);
    printf(") ");
}

void PrintUint(const Uint n)
{
    printf("%zu", n);
}

void PrintUintWRAP(const void* n, const void* sep)
{
    PrintUint(*(const Uint *)n);
    PrintCstr(sep);
}

void PrintUintN(const void* n)
{
    PrintUintWRAP(n, NEW_LINE);
}

void PrintUintS(const void* n)
{
    PrintUintWRAP(n, SPACE);
}

void PrintByte(Byte b)
{
    printf("%u", b);
}

void PrintChar(char c)
{
    printf("%c", c);
}

void PrintCharWRAP(const void* c, const void* sep)
{
    PrintChar(*(const char *)c);
    PrintCstr(sep);
}

void PrintCharS(const void* c)
{
    PrintCharWRAP(c, SPACE);
}

void PrintByteWRAP(const void* byte, const void* sep)
{
    PrintByte(*(const Byte *)byte);
    PrintCstr(sep);
}

void PrintByteS(const void* byte)
{
    PrintByteWRAP(byte, SPACE);
}

void PrintByteN(const void* b)
{
    PrintByteWRAP(b, NEW_LINE);
}

void PrintInt(const Int n)
{
    printf("%ld", n);
}

void PrintIntWRAP(const void* n, const void* sep)
{
    PrintInt(*(const Int *)n);
    PrintCstr(sep);
}

void PrintIntN(const void* n)
{
    PrintIntWRAP(n, NEW_LINE);
}

void PrintIntS(const void* n)
{
    PrintIntWRAP(n, SPACE);
}

void PrintFloat(const Float x)
{
    printf("%.2f", x);
}

void PrintFloatWRAP(const void* x, const void* sep)
{
    PrintFloat(*(const Float *)x);
    PrintCstr(sep);
}

void PrintFloatS(const void* x)
{
    PrintFloatWRAP(x, SPACE);
}

void PrintTimeDiff(const void* start, const void* end)
{
    printf("Time elapsed: %f sec\n", (*(time_t *)end - *(time_t *)start) / ((double)CLOCKS_PER_SEC));
}

void PrintNBits(Uint number, Uint n_bits)
{
    int bit;

    if (n_bits == 0)
        return ;
    
    bit = n_bits - 1;
    while (bit >= 0)
    {
        printf("%d", (number & ((Uint)1 << bit)) ? 1 : 0);
        -- bit;
    }
}

void PrintBits(Uint n)
{
    PrintNBits(n, sizeof(Uint) * __CHAR_BIT__);
}

void PrintBlock(const Block* block, void (*print)(const void *))
{
    void (*_print)(void *);

    _print = (void (*)(void *))print;

    return BlockMap(block, 0, BlockNItems(block), _print);
}

void PrintBlockWRAP(const void* block, void (*print)(const void *), const void *sep)
{
    PrintBlock(block, print);
    printf("%s", (const char *)sep);
}

void PrintBlockN(const void* block, void (*print)(const void *))
{
    PrintBlockWRAP(block, print, NEW_LINE);
}

void PrintMatrix(const Matrix* matrix, void (*print)(const void* ))
{
    Uint n_rows;
    Uint n;
    void (*_print)(void *);

    _print = (void (*)(void *))print;
    n_rows = MatrixNRows(matrix);
    n = 0;

    while (n < n_rows)
    {
        printf("[ ");
        MatrixMapRow((Matrix *)matrix, n, _print);
        printf("]");
        printf("\n");

        ++ n;
    }
    printf("\n");
}

void PrintDeckTail(const Deck* deck, void (*print)(const void *), Uint n_items)
{
    void    (*_print)(void *);
    Uint    index;

    if (n_items == 0)
        return ;

    _print = (void (*)(void *))print;
    index = DeckNItems(deck) - n_items;
    DeckMapRange((Deck *)deck, _print, index, n_items);
    printf("\n");
}

void PrintDeck(const Deck* deck, void (*print)(const void *))
{
    if (deck)
        PrintDeckTail(deck, print, DeckNItems(deck));
    else printf("%s\n", NULL_MSG);
}

void PrintTable(const Table* table, void (*print)(const void* ))
{
    void (*_print)(void *);
    Uint n;
    Uint n_rows;

    _print = (void (*)(void *))print;
    n = 0;
    n_rows = TableNRows(table);

    while (n < n_rows)
    {
        TableMapRow((Table *)table, n, _print);
        printf("\n");
        ++ n;
    }
}

void PrintNatural(const Natural* natural)
{
    Int     length;
    char*   digits;

    digits = NaturalGetDigits(natural);
    length = strlen(digits);
    -- length;

    while (length >= 0)
    {
        PrintChar(digits[length]);
        -- length;
    }
}

void PrintNaturalWRAP(const void* n, const char* sep)
{
    PrintNatural(*(const Natural **)n);
    printf("%s", sep);
}

void PrintNaturalN(const void* n)
{
    PrintNaturalWRAP(n, NEW_LINE);
}

void PrintHashTable(const HashTable* table, void (*print)(const void *))
{
    Uint n;
    Uint n_cells;
    void (*_print)(void *);

    _print = (void (*)(void *))print;
    n_cells = HashTableNCells(table);
    n = 0;

    while (n < n_cells)
    {
        printf("%zu: ", n);
        HashTableMapCell((HashTable *)table, n, _print);
        ++ n;
        printf("\n");
    }
}