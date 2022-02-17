#include "rational.h"

#include <stdlib.h>

void RationalInit(Rational* p, Int top, Int bot)
{
    Int gcd;

    if (top < 0 && bot < 0)
    {
        top = -top;
        bot = -bot;
    }

    gcd = MathGCDInt(top, bot);

    p->top = top / gcd;
    p->bot = bot / gcd;
}

Rational* RationalCreate(Int top, Int bot)
{
    Rational* p;

    if ((p = malloc(sizeof(*p))))
    {
        RationalInit(p, top, bot);

        return p;
    }

    return NULL;
}

void RationalScale(Rational* p, Int top, Int bot)
{
    Rational q;

    RationalInit(&q, top, bot);
    RationalMult(p, p, &q);
}

void RationalDestroy(Rational* p)
{
    if (p)
        free(p);
}

Int RationalTop(const Rational* p)
{
    return p->top;
}

Int RationalBot(const Rational* p)
{
    return p->bot;
}

Rational RationalCopy(const Rational* p)
{
    return (Rational){p->top, p->bot};
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

bool RationalIsZero(const Rational* p)
{
    return p->top == 0;
}

bool RationalEquals(const Rational* p, const Rational* q)
{
    Uint p_gcd;
    Uint q_gcd;

    p_gcd = MathGCD(p->top, p->bot);
    q_gcd = MathGCD(q->top, q->bot);

    return ((p->top / p_gcd) == (q->top / q_gcd)) && ((p->bot / p_gcd) == (q->bot / q_gcd));
}

void RationalInv(Rational* target, const Rational* p)
{
    RationalInit(target, p->bot, p->top);
}

void RationalNegate(Rational* target, const Rational* p)
{
    RationalInit(target, -p->top, p->bot);
}

void RationalDiv(Rational* target, const Rational* lhs, const Rational* rhs)
{
    RationalInv(target, rhs);
    RationalMult(target, target, lhs);
}

void RationalDivWRAP(void* target, const void* lhs, const void* rhs)
{
    RationalDiv(target, lhs, rhs);
}

void RationalInvWRAP(void* target, const void* p)
{
    RationalInv(target, p);
}

bool RationalEqualsWRAP(const void* p, const void* q)
{
    return RationalEquals(p, q);
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

void RationalNegateWRAP(void* target, const void* p)
{
    RationalNegate(target, p);
}

bool RationalIsZeroWRAP(const void* p)
{
    return RationalIsZero(p);
}

