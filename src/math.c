#include "declarations.h"

#define FIB_LIMIT (90)

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

Int MathGCDInt(Int a, Int b)
{
    return MathGCD(a < 0 ? -a : a, b < 0 ? -b : b);
}

//lcm(a, b) * gcd(a, b) = a * b
Uint MathLCM(Uint a, Uint b)
{
    return (a * b) / MathGCD(a, b);
}

Uint MathFib(Uint n)
{
    static Uint fib_numbers[FIB_LIMIT];
    
    if (n >= FIB_LIMIT)
        return 0;
    
    if (n == 0)
        return 0;
    
    if (n == 1)
    {
        fib_numbers[n] = 1;
        return 1;
    }

    if (n == 2)
    {
        fib_numbers[n] = 1;
        return 1;
    }

    if (!fib_numbers[n])
        fib_numbers[n] = MathFib(n - 1) + MathFib(n - 2);

    return fib_numbers[n];
}

Deck* MathFactor(Uint n)
{
    Deck*   factors;
    Uint    p;

    factors = DeckCreateUint();
    p = 2;

    while (p <= n)
    {
        while (n % p == 0)
        {
            DeckPushBack(factors, &p);
            n = n / p;
        }

        ++ p;
    }

    return factors;
}