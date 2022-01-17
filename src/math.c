#include "declarations.h"

Uint MathRandom(void)
{
    return random();
}

Uint MathRandomInRange(Uint n)
{
    return (Uint)random() % n;
}