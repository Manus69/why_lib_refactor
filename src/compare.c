#include "why.h"
#include "macro.h"
#include "declarations.h"

Int CompareByte(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, Byte);
}

Int CompareChar(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, char);
}

Int CompareInt(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, Int);
}

Int CompareUint(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, Uint);
}

Int CompareFloat(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, Float);
}

Int CompareCstr(const void* lhs, const void* rhs)
{
    return -strcmp(*(char **)lhs, *(char **)rhs);
}