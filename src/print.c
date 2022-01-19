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

void PrintRational(const void* p)
{
    printf("%ld / %ld", ((Rational *)p)->top, ((Rational *)p)->bot);
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

void PrintUint(const void* n)
{
    printf("%zu", *(Uint *)n);
}

void PrintUintN(const void* n)
{
    PrintUint(n);
    printf("\n");
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

void PrintBlock(const Block* block, Uint index, Uint n_items, void (*print)(const void *))
{
    return BlockMap(block, index, n_items, print);
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
        MatrixMapRow((Matrix *)matrix, n, _print);
        printf("\n");

        ++ n;
    }
}

void PrintDeck(const Deck* deck, void (*print)(const void *))
{
    void (*_print)(void *);

    _print = (void (*)(void *))print;
    DeckMap((Deck *)deck, _print);
}