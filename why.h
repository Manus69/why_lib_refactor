#ifndef WHY_H
#define WHY_H

#include <stdint.h>
#include <complex.h>

#define NOT_FOUND (-1)
#define WHY_MAX (1LU << 32)
#define ARRAY_DEFAULT_CAPACITY (1 << 1)

#define Complex         complex

typedef int_fast64_t    Int;
typedef uint_fast64_t   Uint;
typedef unsigned char   Byte;
typedef double          Float;
typedef void *          Ptr;

typedef struct Rational Rational;

typedef struct Block    Block;
typedef struct Array    Array;
typedef struct List     List;
typedef struct Tree     Tree;

enum Status
{
    WHY_OK = 0, WHY_ERROR, 
};

#define     BlockCreate(n_items, type) BlockCreate ## type(n_items)
Block*      BlockCreateByte(Uint n_items);
Block*      BlockCreateInt(Uint n_items);
Block*      BlockCreateUint(Uint n_items);
Block*      BlockCreateFloat(Uint n_items);
Block*      BlockCreateRational(Uint n_items);
Block*      BlockCreateComplex(Uint n_items);
Block*      BlockCreatePtr(Uint n_items);

Uint        BlockGetSize(const Block* block);
Uint        BlockGetNItems(const Block* block);
Int         BlockExpand(Block* block, Uint extra_items);
void        BlockDestroy(Block* block);
void        BlockGet(void* target, const Block* block, Uint index);
void        BlockSwap(Block* block, Uint j, Uint k);
void*       BlockPointAt(const Block* block, Uint index);

#define     BlockSet(block, index, item) _Generic((item), \
            Byte : BlockSetByte, \
            Int : BlockSetInt, \
            Uint : BlockSetUint, \
            Float : BlockSetFloat, \
            Rational : BlockSetRational, \
            Complex : BlockSetComplex, \
            default : BlockSetPtr)(block, index, item)

void        BlockSetByte(Block* block, Uint index, Byte value);
void        BlockSetInt(Block* block, Uint index, Int n);
void        BlockSetUint(Block* block, Uint index, Uint n);
void        BlockSetFloat(Block* block, Uint index, Float x);
void        BlockSetComplex(Block* block, Uint index, Complex z);
void        BlockSetRational(Block* block, Uint index, Rational p);
void        BlockSetPtr(Block* block, Uint index, void* ptr);

#define     ArrayCreate(type) ArrayCreate ## type(ARRAY_DEFAULT_CAPACITY)
Array*      ArrayCreateByte(Uint item_capacity);
Array*      ArrayCreateInt(Uint item_capacity);
Array*      ArrayCreateUint(Uint item_capacity);
Array*      ArrayCreateFloat(Uint item_capacity);
Array*      ArrayCreateRational(Uint item_capacity);
Array*      ArrayCreateComplex(Uint item_capacity);
Array*      ArrayCreatePtr(Uint item_capacity);

void        ArrayDestroy(Array* array);
Uint        ArrayGetSize(const Array* array);
void        ArrayGet(void* target, const Array* array, Uint index);
void        ArraySwap(Array* array, Uint j, Uint k); //make it private later

#define     ArraySet(array, index, item) _Generic((item), \
            Byte : ArraySetByte, \
            Int : ArraySetInt, \
            Uint : ArraySetUint, \
            Float : ArraySetFloat, \
            Rational : ArraySetRational, \
            Complex : ArraySetComplex, \
            default : ArraySetPtr)(array, index, item)

void        ArraySetByte(Array* array, Uint index, Byte value);
void        ArraySetInt(Array* array, Uint index, Int n);
void        ArraySetUint(Array* array, Uint index, Uint n);
void        ArraySetFloat(Array* array, Uint index, Float x);
void        ArraySetRational(Array* array, Uint index, Rational p);
void        ArraySetComplex(Array* array, Uint index, Complex z);
void        ArraySetPtr(Array* array, Uint index, void* item);
    
void        RationalInit(Rational* p, Int top, Int bot);

Int         CompareInt(const void* lhs, const void* rhs);
Int         ComapreFloat(const void* lhs, const void* rhs);
Int         CompareCstr(const void* lhs, const void* rhs);

Byte*       ReadFile(const char* name);
Block*      ReadFileByLine(const char* name);

void        PrintRational(const Rational* p);

#endif