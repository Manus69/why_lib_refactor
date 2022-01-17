#include "rational.h"

void RationalInit(Rational* p, Int top, Int bot)
{
    p->top = top;
    p->bot = bot;
}

void RationalZero(Rational* p)
{
    RationalInit(p, 0, 1);
}

void RationalOne(Rational* p)
{
    RationalInit(p, 1, 1);
}

void RationalAdd(Rational* p, const Rational* lhs, const Rational* rhs)
{
    RationalInit(p, lhs->top * rhs->bot + rhs->top * lhs->bot, lhs->bot * rhs->bot);
}

void RationalMult(Rational* p, const Rational* lhs, const Rational* rhs)
{
    RationalInit(p, lhs->top * rhs->top, lhs->bot * rhs->bot);
}

void RationalZeroWRAP(void* target)
{
    RationalZero(target);
}

void RationalOneWRAP(void* target)
{
    RationalOne(target);
}

void RationalAddWRAP(void* target, const void* lhs, const void* rhs)
{
    RationalAdd(target, lhs, rhs);
}

void RationalMultWRAP(void* target, const void* lhs, const void* rhs)
{
    RationalMult(target, lhs, rhs);
}

