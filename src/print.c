#include "why.h"
#include "macro.h"
#include "rational.h"

#include <stdio.h>

void PrintRational(const Rational* p)
{
    printf("%ld / %ld", p->top, p->bot);
}
