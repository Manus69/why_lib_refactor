#include "declarations.h"

void FloatZero(Float* x)
{
    *x = 0.0;
}

void FloatOne(Float* x)
{
    *x = 1.0;
}

void FloatAdd(Float* x, const Float* lhs, const Float* rhs)
{
    *x = *lhs + *rhs;
}

void FloatMult(Float* x, const Float* lhs, const Float* rhs)
{
    *x = *lhs * *rhs;
}

void FloatZeroWRAP(void* target)
{
    FloatZero(target);
}

void FloatOneWRAP(void* target)
{
    FloatOne(target);
}

void FloatAddWRAP(void* target, const void* lhs, const void* rhs)
{
    FloatAdd(target, lhs, rhs);
}

void FloatMultWRAP(void* target, const void* lhs, const void* rhs)
{
    FloatMult(target, lhs, rhs);
}