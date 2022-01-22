#ifndef WHY_H
#define WHY_H

#include <stdint.h>
#include <complex.h>
#include <stdbool.h>

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
    WHY_OK = 0, WHY_ERROR = -1, 
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
    void (*negate)(void* target, const void* number);
    void (*invert)(void* target, const void* number);
    void (*add)(void* target, const void* lhs, const void* rhs);
    void (*mult)(void* target, const void* lhs, const void* rhs);
    void (*div)(void* target, const void* lhs, const void* rhs);
    bool (*equals)(const void* lhs, const void* rhs);
    bool (*is_zero)(const void* number);
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
Rational    RationalCopy(const Rational* p);
void        RationalZero(Rational* p);
void        RationalOne(Rational* p);
void        RationalAdd(Rational* p, const Rational* lhs, const Rational* rhs);
void        RationalMult(Rational* p, const Rational* lhs, const Rational* rhs);
void        RationalDiv(Rational* target, const Rational* lhs, const Rational* rhs);
void        RationalInv(Rational* target, const Rational* p);
bool        RationalEquals(const Rational* p, const Rational* q);
void        RationalNegate(Rational* target, const Rational* p);
bool        RationalEqualsWRAP(const void* p, const void* q);
void        RationalZeroWRAP(void* target);
void        RationalOneWRAP(void* target);
void        RationalAddWRAP(void* target, const void* lhs, const void* rhs);
void        RationalMultWRAP(void* target, const void* lhs, const void* rhs);
bool        RationalIsZeroWRAP(const void* p);
void        RationalInvWRAP(void* target, const void* p);
void        RationalNegateWRAP(void* target, const void* p);
void        RationalDivWRAP(void* target, const void* lhs, const void* rhs);

void        FloatZero(Float* x);
void        FloatOne(Float* x);
void        FloatAdd(Float* x, const Float* lhs, const Float* rhs);
void        FloatMult(Float* x, const Float* lhs, const Float* rhs);
void        FloatDiv(Float* target, const Float* top, const Float* bot);
bool        FloatEquals(const Float* x, const Float* y);
bool        FloatIsZero(const Float* x);
void        FloatNegate(Float* target, const Float* x);
void        FloatInv(Float* target, const Float* x);
void        FloatZeroWRAP(void* target);
void        FloatOneWRAP(void* target);
void        FloatAddWRAP(void* target, const void* lhs, const void* rhs);
void        FloatMultWRAP(void* target, const void* lhs, const void* rhs);
void        FloatDivWRAP(void* target, const void* top, const void* bot);
void        FloatABS(Float* target, const Float* x);
bool        FloatEqualsWRAP(const void* x, const void* y);
bool        FloatIsZeroWRAP(const void* x);
void        FloatNegateWRAP(void* target, const void* x);
void        FloatInvWRAP(void* target, const void* x);

void        UintInit(Uint* target, Uint value);

void        IntInit(Int* target, Int value);

Int         CompareInt(const void* lhs, const void* rhs);
Int         CompareUint(const void* lhs, const void* rhs);
Int         ComapreFloat(const void* lhs, const void* rhs);
Int         CompareCstr(const void* lhs, const void* rhs);

Uint        MathRandom(void);
Uint        MathRandomInRange(Uint n);
Uint        MathGCD(Uint a, Uint b);
Int         MathGCDInt(Int a, Int b);

Matrix*     MatrixCreateFloat(Uint n_rows, Uint n_cols);
Matrix*     MatrixCreateRational(Uint n_rows, Uint n_cols);
void        MatrixDestroy(Matrix* matrix);
void        MatrixInitFromArray(Matrix* matrix, const void* array);
Uint        MatrixNRows(const Matrix* matrix);
Uint        MatrixNCols(const Matrix* matrix);
Uint        MatrixNItems(const Matrix* matrix);
void        MatrixGet(void* target, const Matrix* matrix, Uint row, Uint col);
void        MatrixSetNth(Matrix* matrix, Uint n, const void* item);
void        MatrixSet(Matrix* matrix, Uint row, Uint col, const void* item);
void*       MatrixPointAt(const Matrix* matrix, Uint row, Uint col);
void        MatrixDot(void* target, const Matrix* lhs, const Matrix* rhs, Uint row, Uint col);
Int         MatrixMult(Matrix* target, const Matrix* lhs, const Matrix* rhs);
Int         MatrixAdd(Matrix* target, const Matrix* lhs, const Matrix* rhs);
void        MatrixMapRow(Matrix* matrix, Uint row, void (*function)(void* ));
//these should be made private
void        MatrixSwapRows(Matrix* matrix, Uint j, Uint k);
void        MatrixScaleRow(Matrix* matrix, Uint row, const void* value);
void        MatrixAddRows(Matrix* matrix, Uint j, Uint k);
void        MatrixAddScaledRows(Matrix* matrix, Uint target, Uint source, const void* factor);
Int         MatrixFindPivotRow(const Matrix* matrix);
void        MatrixRowEliminate(Matrix* matrix, Uint pivot_row, Uint target_row);
void        MatrixEchelonForm(Matrix* matrix);
void        MatrixEliminateUp(Matrix* matrix);

bool        IsDigit(char c);
bool        IsAlpha(char c);
bool        IsSpace(char c);
Int         ParseUint(Uint* target, const char* string);
Int         ParseInt(Int* target, const char* string);
Int         ParseRational(Rational* target, const char* string);

Byte*       ReadFile(const char* name);
Deck*       ReadFileAllLines(const char* name);

void        PrintInt(const void* n);
void        PrintIntN(const void* n);
void        PrintIntS(const void* n);
void        PrintCstr(const void* str);
void        PrintCstrN(const void* str);
void        PrintTimeDiff(long start, long end);
void        PrintRational(const void* p);
void        PrintRationalN(const void* p);
void        PrintRationalS(const void* p);
void        PrintRationalP(const void* p);
void        PrintFloat(const void* x);
void        PrintFloatS(const void* x);
void        PrintUintN(const void* n);
void        PrintNBits(Uint number, Uint n_bits);
void        PrintBits(Uint n);
void        PrintBlock(const Block* block, Uint index, Uint n_items, void (*print)(const void *));
void        PrintMatrix(const Matrix* matrix, void (*print)(const void* ));
void        PrintMatrixN(const void* matrix, void (*print)(const void *));
void        PrintDeck(const Deck* deck, void (*print)(const void *));  

#endif