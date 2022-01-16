#include "why.h"
#include "macro.h"
#include "declarations.h"

Int CompareInt(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, Int);
}

Int ComapreFloat(const void* lhs, const void* rhs)
{
    return COMPARE_NUMERIC(lhs, rhs, Float);
}

Int CompareCstr(const void* lhs, const void* rhs)
{
    return -strcmp(*(char **)lhs, *(char **)rhs);
}