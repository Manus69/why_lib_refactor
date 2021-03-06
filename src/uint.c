#include "declarations.h"

void UintInit(Uint* target, Uint value)
{
    *target = value;
}

void UintAdd(Uint* target, const Uint* lhs, const Uint* rhs)
{
    UintInit(target, *lhs + *rhs);
}

void UintAddWRAP(void* target, const void* lhs, const void* rhs)
{
    UintAdd(target, lhs, rhs);
}

void UintMult(Uint* target, const Uint* lhs, const Uint* rhs)
{
    UintInit(target, *lhs * *rhs);
}

void UintMultWRAP(void* target, const void* lhs, const void* rhs)
{
    UintMult(target, lhs, rhs);
}

Uint UintToStringBuffer(char* buffer, Uint n, Uint base)
{
    Uint index;

    if (n == 0)
    {
        *buffer = '0';
        ++ buffer;
        *buffer = 0;

        return 1;
    }

    index = 0;
    while (n)
    {
        buffer[index] = DIGITS[n % base];
        n /= base;
        ++ index;
    }

    buffer[index] = 0;
    StringReverseLength(buffer, index);

    return index;
}

#define BUFFER_SIZE (20)
char* UintToString(Uint n)
{
    char* buffer;

    if ((buffer = malloc(BUFFER_SIZE)))
    {
        UintToStringBuffer(buffer, n, 10);

        return buffer;
    }

    return NULL;
}

Uint Popcount(Uint n)
{
    Uint count;

    count = 0;

    while (n)
    {
        ++ count;
        n = n & (n - 1);
    }

    return count;
}