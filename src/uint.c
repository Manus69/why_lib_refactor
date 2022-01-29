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
