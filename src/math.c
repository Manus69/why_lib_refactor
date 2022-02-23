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

Uint MathGCDArray(const Uint* array, Uint size)
{
    Uint gcd;

    if (size == 0)
        return 0;
    
    if (size == 1)
        return *array;
    
    if (size == 2)
        return MathGCD(array[0], array[1]);
    
    gcd = MathGCD(array[0], array[1]);

    return MathGCD(gcd, MathGCDArray(++ array, -- size));
}

//lcm(a, b) * gcd(a, b) = a * b
Uint MathLCM(Uint a, Uint b)
{
    return (a * b) / MathGCD(a, b);
}

Uint MathLCMArray(const Uint* array, Uint size)
{
    Uint lcm;

    if (size == 0)
        return 0;

    if (size == 1)
        return *array;
    
    lcm = MathLCM(array[0], array[1]);

    if (size == 2)
        return lcm;
    
    return MathLCM(lcm, MathLCMArray(++ array, -- size));
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

Uint MathFactorial(Uint n)
{
    Uint result;

    result = 1;
    while (n)
    {
        result *= n;
        -- n;
    }

    return result;
}

Uint MathCountDigits(Uint n)
{
    Uint count;

    if (n == 0)
        return 1;
    
    count = 0;
    while (n)
    {
        n = n / 10;
        ++ count;
    }

    return count;
}

Uint MathPower(Uint base, Uint exp)
{
    if (exp == 0)
        return 1;
    
    if (exp == 1)
        return base;
    
    if (exp % 2 == 0)
        return MathPower(base * base, exp / 2);
    
    return base * MathPower(base * base, (exp - 1) / 2);
}

void MathPascalsTriangleFill(Uint n, Uint array[n][n])
{
    Uint j, k;

    j = 0;
    while (j < n)
    {
        k = 0;
        while (k < n)
        {
            if (k == j)
                array[j][k] = 1;
            else if (k > j)
                array[j][k] = 0;
            else if (k == 0)
                array[j][k] = 1;
            else
                array[j][k] = array[j - 1][k - 1] + array[j - 1][k];

            ++ k;
        }

        ++ j;
    }
}