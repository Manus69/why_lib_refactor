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

void FloatSubt(Float* x, const Float* lhs, const Float* rhs)
{
    *x = *lhs - *rhs;
}

void FloatMult(Float* x, const Float* lhs, const Float* rhs)
{
    *x = *lhs * *rhs;
}

void FloatABS(Float* target, const Float* x)
{
    *target = *x > 0 ? *x : -*x;
}

void FloatDiv(Float* target, const Float* top, const Float* bot)
{
    *target = *top / *bot;
}

void FloatNegate(Float* target, const Float* x)
{
    *target = - *x;
}

void FloatInv(Float* target, const Float* x)
{
    *target = 1 / *x;
}

void FloatDivWRAP(void* target, const void* top, const void* bot)
{
    FloatDiv(target, top, bot);
}

bool FloatEquals(const Float* x, const Float* y)
{
    Float result;

    result = *x - *y;
    result = result > 0 ? result : -result;

    return result < FLT_EPSILON;
}

bool FloatIsZero(const Float* x)
{
    Float zero;

    zero = 0.0;

    return FloatEquals(x, &zero);
}

bool FloatEqualsWRAP(const void* x, const void* y)
{
    return FloatEquals(x, y);
}

bool FloatIsZeroWRAP(const void* x)
{
    return FloatIsZero(x);
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

void FloatNegateWRAP(void* target, const void* x)
{
    FloatNegate(target, x);
}

void FloatInvWRAP(void* target, const void* x)
{
    FloatInv(target, x);
}