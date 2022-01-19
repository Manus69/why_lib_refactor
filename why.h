#ifndef WHY_H
#define WHY_H

#include <stdint.h>
#include <complex.h>

#define NOT_FOUND (-1)
#define WHY_MAX (1LU << 32)
#define ARRAY_DEFAULT_CAPACITY (1 << 1)

#define Complex         complex

typedef int_fast64_t            Int;
typedef uint_fast64_t           Uint;
typedef unsigned char           Byte;
typedef double                  Float;
typedef void *                  Ptr;

typedef struct TypeInterface    TypeInterface;
typedef struct ArInterface      ArInterface;
typedef struct Rational         Rational;

typedef struct Block            Block;
typedef struct Deck             Deck;
typedef struct List             List;
typedef struct Tree             Tree;
typedef struct Matrix           Matrix;

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

extern const TypeInterface  PtrInterface;
extern const TypeInterface  ByteInterface;
extern const TypeInterface  IntInterface;
extern const TypeInterface  UintInterface;
extern const TypeInterface  FloatInterface;
extern const TypeInterface  RationalInterface;
extern const TypeInterface  ComplexInterface;

extern const ArInterface    RationalArInterface;
extern const ArInterface    FloatArInterface;

Int         WhyStart(void);
void        WhyEnd(void);
Int         WhySavePtr(const void* ptr);

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
Uint        BlockNItems(const Block* block);
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

Deck*       DeckCreatePtr(void* (*copy)(const void *), void (*destroy)(void *));
void        DeckDestroy(Deck* deck);
Uint        DeckNItems(const Deck* deck);
void        DeckSet(Deck* deck, Uint index, const void* item);
void*       DeckPointAt(Deck* deck, Uint index);
Int         DeckPushBack(Deck* deck, const void* item);
Int         DeckPushFront(Deck* deck, const void* item);
void        DeckMap(Deck* deck, void (*function)(void *));
void        DeckReserve(Deck* deck, Uint n_items);
Int         DeckLast(void* target, const Deck* deck);
Int         DeckFirst(void* target, const Deck* deck);

void        InsertionSort(Block* block, Int left_index, Int right_index, Int (*compare)(const void *, const void *));
void        QuickSort(Block* block, Int left_index, Int right_index, Int (*compare)(const void *, const void *));
void        SortDeck(Deck* deck, Int (*compare)(const void *, const void *));

void        RationalInit(Rational* p, Int top, Int bot);
void        RationalZero(Rational* p);
void        RationalOne(Rational* p);
void        RationalAdd(Rational* p, const Rational* lhs, const Rational* rhs);
void        RationalMult(Rational* p, const Rational* lhs, const Rational* rhs);
void        RationalZeroWRAP(void* target);
void        RationalOneWRAP(void* target);
void        RationalAddWRAP(void* target, const void* lhs, const void* rhs);
void        RationalMultWRAP(void* target, const void* lhs, const void* rhs);

void        FloatZero(Float* x);
void        FloatOne(Float* x);
void        FloatAdd(Float* x, const Float* lhs, const Float* rhs);
void        FloatMult(Float* x, const Float* lhs, const Float* rhs);
void        FloatZeroWRAP(void* target);
void        FloatOneWRAP(void* target);
void        FloatAddWRAP(void* target, const void* lhs, const void* rhs);
void        FloatMultWRAP(void* target, const void* lhs, const void* rhs);

Int         CompareInt(const void* lhs, const void* rhs);
Int         CompareUint(const void* lhs, const void* rhs);
Int         ComapreFloat(const void* lhs, const void* rhs);
Int         CompareCstr(const void* lhs, const void* rhs);

Uint        MathRandom(void);
Uint        MathRandomInRange(Uint n);

Matrix*     MatrixCreateFloat(Uint n_rows, Uint n_cols);
Matrix*     MatrixCreateRational(Uint n_rows, Uint n_cols);
void        MatrixDestroy(Matrix* matrix);
Uint        MatrixNRows(const Matrix* matrix);
Uint        MatrixNCols(const Matrix* matrix);
void        MatrixGet(void* target, const Matrix* matrix, Uint row, Uint col);
void        MatrixSetNth(Matrix* matrix, Uint n, const void* item);
void        MatrixSet(Matrix* matrix, Uint row, Uint col, const void* item);
void*       MatrixPointAt(const Matrix* matrix, Uint row, Uint col);
void        MatrixDot(void* target, const Matrix* lhs, const Matrix* rhs, Uint row, Uint col);
Int         MatrixMult(Matrix* target, const Matrix* lhs, const Matrix* rhs);
Int         MatrixAdd(Matrix* target, const Matrix* lhs, const Matrix* rhs);
void        MatrixMapRow(Matrix* matrix, Uint row, void (*function)(void* ));

Byte*       ReadFile(const char* name);
Deck*       ReadFileAllLines(const char* name);

void        PrintCstr(const void* str);
void        PrintCstrN(const void* str);
void        PrintTimeDiff(long start, long end);
void        PrintRational(const void* p);
void        PrintRationalN(const void* p);
void        PrintRationalS(const void* p);
void        PrintFloat(const void* x);
void        PrintFloatS(const void* x);
void        PrintUintN(const void* n);
void        PrintNBits(Uint number, Uint n_bits);
void        PrintBits(Uint n);
void        PrintBlock(const Block* block, Uint index, Uint n_items, void (*print)(const void *));
void        PrintMatrix(const Matrix* matrix, void (*print)(const void* ));
void        PrintDeck(const Deck* deck, void (*print)(const void *));  

#endif