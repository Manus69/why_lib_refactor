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

void UintToStringBuffer(char* buffer, Uint n, Uint base)
{
    Uint index;

    if (n == 0)
    {
        *buffer = '0';
        ++ buffer;
        *buffer = 0;

        return ;
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