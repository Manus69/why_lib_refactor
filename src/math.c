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

bool MathIsPrime(Uint n)
{
    Uint p;

    p = 2;
    while (p * p <= n)
    {
        if (n % p == 0)
            return false;
        ++ p;
    }

    return true;
}

Deck* MathFactor(Uint n)
{
    Deck*   factors;
    Uint    p;

    if (!(factors = DeckCreateUint()))
        return NULL;
    
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

static void _generate_divisors(const Deck* prime_divisors, Deck* divisors, Uint current, Uint index)
{
    Uint divisor;

    if (index == DeckNItems(prime_divisors))
    {
        DeckPushBack(divisors, &current);
        return ;
    }
    
    divisor = *(Uint *)DeckPointAt(prime_divisors, index);
    divisor *= current;

    _generate_divisors(prime_divisors, divisors, current, index + 1);
    _generate_divisors(prime_divisors, divisors, divisor, index + 1);
}

Deck* MathComputeDivisors(Uint n)
{
    Deck*   divisors;
    Deck*   prime_divisors;
    
    if (!(prime_divisors = MathFactor(n)))
        return NULL;
    
    if (!(divisors = DeckCreateUint()))
    {
        DeckDestroy(prime_divisors);
        return NULL;
    }

    _generate_divisors(prime_divisors, divisors, 1, 0);

    DeckDestroy(prime_divisors);

    return divisors;
}

static void _sieve_init(Block* sieve, Uint start)
{
    Uint size;
    Byte one;

    size = BlockNItems(sieve);
    one = 1;

    while (start < size)
    {
        BlockSet(sieve, start, &one);
        ++ start;
    }
}

static Uint _get_next_prime(const Block* sieve, Uint index)
{
    Uint size;
    Byte current;

    ++ index;
    size = BlockNItems(sieve);

    while (index < size)
    {
        BlockGet(&current, sieve, index);
        if (current)
            return index;
        
        ++ index;
    }

    return 0;
}

static void _eliminate_multiples(Block* sieve, Uint p, Uint start)
{
    Uint size;
    Byte zero;

    zero = 0;
    size = BlockNItems(sieve);
    
    while (start < size)
    {
        BlockSet(sieve, start, &zero);
        start += p;
    }
}

Block* MathSieve(Uint size)
{
    Block*  sieve;
    Uint    p;

    if (!(sieve = BlockCreateByte(size)))
        return NULL;
    
    _sieve_init(sieve, 0);
    p = 0;
    BlockSet(sieve, 0, &p);
    BlockSet(sieve, 1, &p);

    p = 2;
    while (2 * p <= size)
    {
        _eliminate_multiples(sieve, p, p + p);
        p = _get_next_prime(sieve, p);
    }

    return sieve;
}

Deck* MathGetNPrimes(Uint n)
{
    Deck*   primes;
    Uint    prime;

    if (!(primes = DeckCreateUint()))
        return NULL;
    
    prime = 2;
    DeckPushBack(primes, &prime);
    ++ prime;

    while (DeckNItems(primes) < n)
    {
        if (MathIsPrime(prime))
            DeckPushBack(primes, &prime);
        
        ++ prime;
    }

    return primes;
}

Uint MathGetNthPrime(Uint n)
{
    Block*  sieve;
    Uint    index;
    Uint    count;
    Byte    current;

    if (!(sieve = MathSieve(20 * n))) //get a better formula
        return 0;

    index = 0;
    count = 0;

    while (true)
    {
        BlockGet(&current, sieve, index);

        if (current)
            ++ count;

        if (n == count)
            break ;
        
        ++ index;
    }

    BlockDestroy(sieve);

    return index;
}