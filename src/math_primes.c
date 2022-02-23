#include "declarations.h"

static Uint     _zero = 0;
static Uint     _one = 1;
static Block*   _sieve = NULL;

Int MathUnitInit(Uint sieve_size)
{
    (void)_zero;

    if (!(_sieve = MathSieve(sieve_size)))
        return WHY_ERROR;

    return WHY_OK;
}

void MathUnitTerminate()
{
    BlockDestroy(_sieve);
}

Block* MathGetSieve()
{
    return _sieve;
}

bool MathIsPrime(Uint n)
{
    Uint p;
    Byte value;

    if (_sieve && (n < BlockNItems(_sieve)))
    {
        BlockGet(&value, _sieve, n);

        return value;
    }

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
    Deck*   result;
    
    if (!(prime_divisors = MathFactor(n)))
        return NULL;
    
    if (n == 1)
    {
        DeckPushBack(prime_divisors, &_one);
        return prime_divisors;
    }

    if (DeckNItems(prime_divisors) == 0)
        return prime_divisors;
    
    if (!(divisors = DeckCreateUint()))
    {
        DeckDestroy(prime_divisors);
        return NULL;
    }

    _generate_divisors(prime_divisors, divisors, 1, 0);
    DeckDestroy(prime_divisors);

    if (!(result = DeckUnique(divisors, CompareUint)))
    {
        DeckDestroy(divisors);
        return NULL;
    }

    DeckDestroy(divisors);

    return result;
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

static Int _add_three(Deck* primes)
{
    Uint p;

    p = 3;

    return DeckPushBack(primes, &p);
}

Int MathGetNextPrime(Deck* primes)
{
    Uint next;
    Uint p;
    Uint n_items;
    Uint index;

    if (DeckLast(&next, primes) == WHY_ERROR)
        return WHY_ERROR;
    
    n_items = DeckNItems(primes);

    if (n_items == 1)
        return _add_three(primes);
    
    next += 2;
    index = 1;
    while (index < n_items)
    {
        p = *(Uint *)DeckPointAt(primes, index);
        if (next % p == 0)
        {
            index = 1;
            next += 2;
            continue ;
        }
        
        ++ index;
    }

    return DeckPushBack(primes, &next);
}

Deck* MathGetPrimesUpToN(Uint n)
{
    Uint    current;
    Deck*   primes;

    if (!(primes = DeckCreateUintCapacity(n)))
        return NULL;
    
    current = 2;
    DeckPushBack(primes, &current);

    ++ current;
    while (current <= n)
    {
        if (MathIsPrime(current))
        {
            DeckPushBack(primes, &current);
        }

        current += 2;
    }

    return primes;
}