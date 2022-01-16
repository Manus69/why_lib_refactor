#include "why.h"
#include "macro.h"
#include "rational.h"
#include "declarations.h"

void PrintRational(const Rational* p)
{
    printf("%ld / %ld", p->top, p->bot);
}
