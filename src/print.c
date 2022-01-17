#include "why.h"
#include "macro.h"
#include "rational.h"
#include "declarations.h"

void PrintCstr(const char* str)
{
    if (!str)
        printf("NULL");
    else
        printf("%s", str);
}

void PrintCstrN(const char* str)
{
    PrintCstr(str);
    printf("\n");
}

void PrintRational(const Rational* p)
{
    printf("%ld / %ld", p->top, p->bot);
}

void PrintRationalN(const Rational* p)
{
    PrintRational(p);
    printf("\n");
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