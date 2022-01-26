#include "why.h"
#include "macro.h"
#include "rational.h"
#include "declarations.h"

void PrintCstr(const void* str)
{
    if (!str)
        printf("NULL");
    else
        printf("%s", *(char **)str);
}

void PrintCstrN(const void* str)
{
    PrintCstr(str);
    printf("\n");
}

void PrintCstrS(const void* str)
{
    PrintCstr(str);
    printf(" ");
}

void PrintRational(const void* p)
{
    Int top;
    Int bot;

    top = ((Rational *)p)->top;
    bot = ((Rational *)p)->bot;
    if (bot == 1)
        printf("%ld", top);
    else if (bot == -1)
        printf("%ld", -top);
    else
        printf("%ld/%ld", ((Rational *)p)->top, ((Rational *)p)->bot);
}

void PrintRationalN(const void* p)
{
    PrintRational(p);
    printf("\n");
}

void PrintRationalS(const void* p)
{
    PrintRational(p);
    printf(" ");
}

void PrintRationalP(const void* p)
{
    printf("(");
    PrintRational(p);
    printf(") ");
}

void PrintUint(const void* n)
{
    printf("%zu", *(Uint *)n);
}

void PrintUintN(const void* n)
{
    PrintUint(n);
    printf("\n");
}

void PrintByte(const void* b)
{
    printf("%u", *(Byte *)b);
}

void PrintByteN(const void* b)
{
    PrintByte(b);
    printf("\n");
}

void PrintInt(const void* n)
{
    printf("%ld", *(Int *)n);
}

void PrintIntN(const void* n)
{
    PrintInt(n);
    printf("\n");
}

void PrintIntS(const void* n)
{
    PrintInt(n);
    printf(" ");
}

void PrintFloat(const void* x)
{
    printf("%.2f", *(Float *)x);
}

void PrintFloatS(const void* x)
{
    PrintFloat(x);
    printf(" ");
}

void PrintTimeDiff(time_t start, time_t end)
{
    printf("Time elapsed: %f sec\n", (end - start) / ((double)CLOCKS_PER_SEC));
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
    return BlockMap(block, 0, BlockNItems(block), print);
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

void PrintMatrixN(const void* matrix, void (*print)(const void *))
{
    PrintMatrix(matrix, print);
    printf("\n");
}

void PrintDeck(const Deck* deck, void (*print)(const void *))
{
    void (*_print)(void *);

    if (!deck)
        return ;
    
    _print = (void (*)(void *))print;
    DeckMap((Deck *)deck, _print);
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