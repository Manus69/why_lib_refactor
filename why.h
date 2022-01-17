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

typedef struct TypeInterface TypeInterface;
typedef struct ArInterface ArInterface;
typedef struct Rational Rational;

typedef struct Block    Block;
typedef struct Array    Array;
typedef struct List     List;
typedef struct Tree     Tree;
typedef struct Matrix   Matrix;

enum Status
{
    WHY_OK = 0, WHY_ERROR, 
};

struct TypeInterface
{
    void (*get)(void* target, const void* memory, Uint index);
    void (*set)(void* memory, Uint index, const void* ptr);
    void (*swap)(void* memory, Uint j, Uint k);
    Uint size;
};

struct ArInterface
{
    void (*zero)(void* target);
    void (*one)(void* target);
    void (*add)(void* target, const void* lhs, const void* rhs);
    void (*mult)(void* target, const void* lhs, const void* rhs);
};

extern const TypeInterface PtrInterface;
extern const TypeInterface ByteInterface;
extern const TypeInterface IntInterface;
extern const TypeInterface UintInterface;
extern const TypeInterface FloatInterface;
extern const TypeInterface RationalInterface;
extern const TypeInterface ComplexInterface;

extern const ArInterface RationalArInterface;

Int         WhyStart(void);
void        WhyEnd(void);

void        GetPtr(void* target, const void* memory, Uint index);
void        SetPtr(void* memory, Uint index, const void* ptr);
void        SwapPtr(void* memory, Uint j, Uint k);
void        GetByte(void* target, const void* memory, Uint index);
void        SetByte(void* memory, Uint index, const void* ptr);
void        SwapByte(void* memory, Uint j, Uint k);
void        GetInt(void* target, const void* memory, Uint index);
void        SetInt(void* memory, Uint index, const void* ptr);
void        SwapInt(void* memory, Uint j, Uint k);
void        GetUint(void* target, const void* memory, Uint index);
void        SetUint(void* memory, Uint index, const void* ptr);
void        SwapUint(void* memory, Uint j, Uint k);
void        GetFloat(void* target, const void* memory, Uint index);
void        SetFloat(void* memory, Uint index, const void* ptr);
void        SwapFloat(void* memory, Uint j, Uint k);
void        GetRational(void* target, const void* memory, Uint index);
void        SetRational(void* memory, Uint index, const void* ptr);
void        SwapRational(void* memory, Uint j, Uint k);
void        GetComplex(void* target, const void* memory, Uint index);
void        SetComplex(void* memory, Uint index, const void* ptr);
void        SwapComplex(void* memory, Uint j, Uint k);

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
void        BlockDestroyNoMem(Block* block);
void*       BlockDestroyReturnContent(Block* block);
void*       BlockGetContent(const Block* block);
void*       BlockPointAt(const Block* block, Uint index);
Int         BlockCopyItems(Block* block, Uint index, const void* pointer, Uint n_items);
void        BlockGet(void* target, const Block* block, Uint index);
void        BlockSet(Block* block, Uint index, const void* item);
void        BlockSwap(Block* block, Uint j, Uint k);
Int         BlockCompare(const Block* block, Uint j, Uint k, Int (*compare)(const void* lhs, const void* rhs));
void        BlockMap(const Block* block, Uint index, Uint n_items, void (*function)(const void *));

void        QuickSort(Block* block, Int left_index, Int right_index, Int (*compare)(const void* lhs, const void* rhs));

void        RationalInit(Rational* p, Int top, Int bot);
void        RationalZero(Rational* p);
void        RationalOne(Rational* p);
void        RationalAdd(Rational* p, const Rational* lhs, const Rational* rhs);
void        RationalMult(Rational* p, const Rational* lhs, const Rational* rhs);
void        RationalZeroWRAP(void* target);
void        RationalOneWRAP(void* target);
void        RationalAddWRAP(void* target, const void* lhs, const void* rhs);
void        RationalMultWRAP(void* target, const void* lhs, const void* rhs);

Int         CompareInt(const void* lhs, const void* rhs);
Int         CompareUint(const void* lhs, const void* rhs);
Int         ComapreFloat(const void* lhs, const void* rhs);
Int         CompareCstr(const void* lhs, const void* rhs);

Uint        MathRandom(void);
Uint        MathRandomInRange(Uint n);

Byte*       ReadFile(const char* name);
Array*      ReadFileByLine(const char* name);

void        PrintCstr(const char* str);
void        PrintCstrN(const char* str);
void        PrintTimeDiff(long start, long end);
void        PrintRational(const Rational* p);
void        PrintRationalN(const Rational* p);
void        PrintUintN(const void* n);
void        PrintNBits(Uint number, Uint n_bits);
void        PrintBits(Uint n);
void        PrintBlock(const Block* block, Uint index, Uint n_items, void (*print)(const void *));

#endif