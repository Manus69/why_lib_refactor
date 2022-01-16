#ifndef WHY_H
#define WHY_H

#include <stdint.h>
#include <complex.h>

#define NOT_FOUND (-1)
#define WHY_MAX (1LU << 32)

#define Complex         complex

typedef int_fast64_t    Int;
typedef uint_fast64_t   Uint;
typedef unsigned char   Byte;
typedef double          Float;
typedef void *          Ptr;

typedef struct Rational Rational;

typedef struct Array    Array;
typedef struct List     List;
typedef struct Tree     Tree;

enum Status
{
    WHY_OK = 0, WHY_ERROR, 
};

#define ArrayCreate(n_items, type) ArrayCreate ## type(n_items)
Array*  ArrayCreateInt(Uint n_items);
Array*  ArrayCreateUint(Uint n_items);
Array*  ArrayCreateFloat(Uint n_items);
Array*  ArrayCreateRational(Uint n_items);
Array*  ArrayCreateComplex(Uint n_items);
Array*  ArrayCreatePtr(Uint n_items);

void    ArrayDestroy(Array* array);
Uint    ArrayGetSize(const Array* array);
void    ArrayGet(void* target, const Array* array, Uint index);
void    ArraySwap(Array* array, Uint j, Uint k); //make it private later

#define ArraySet(array, index, item) _Generic((item), \
        Int : ArraySetInt, \
        Uint : ArraySetUint, \
        Float : ArraySetFloat, \
        Rational : ArraySetRational, \
        Complex : ArraySetComplex, \
        default : ArraySetPtr)(array, index, item)

void    ArraySetInt(Array* array, Uint index, Int n);
void    ArraySetUint(Array* array, Uint index, Uint n);
void    ArraySetFloat(Array* array, Uint index, Float x);
void    ArraySetRational(Array* array, Uint index, Rational p);
void    ArraySetComplex(Array* array, Uint index, Complex z);
void    ArraySetPtr(Array* array, Uint index, void* item);

void    RationalInit(Rational* p, Int top, Int bot);

void    PrintRational(const Rational* p);

#endif