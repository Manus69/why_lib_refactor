#include "declarations.h"

Uint MathRandom(void)
{
    return random();
}

Uint MathRandomInRange(Uint n)
{
    return (Uint)random() % n;
}

//a = xb + r -> gcd(a, b) = gcd(b, r);
Uint MathGCD(Uint a, Uint b)
{
    Uint r;

    if (a == 0)
        return b;
    
    if (b == 0)
        return a;

    if (b > a)
        return MathGCD(b, a);
    
    r = a % b;

    return MathGCD(b, r);
}