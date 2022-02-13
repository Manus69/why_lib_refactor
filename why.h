#ifndef WHY_H
#define WHY_H

#include <stdint.h>
#include <complex.h>
#include <stdbool.h>

#define NOT_FOUND (-1)
#define WHY_MAX (1LU << 63)
#define ARRAY_DEFAULT_CAPACITY (1 << 1)
#define MAX(x, y) ((x) > (y) ? (x) : (y)) //this is bad
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define TOKEN_PARSE_SUCCESS 1

#define Complex         complex

typedef int_fast64_t            Int;
typedef uint_fast64_t           Uint;
typedef unsigned char           Byte;
typedef double                  Float;
typedef void *                  Ptr;

typedef struct TypeInterface    TypeInterface;
typedef struct ArInterface      ArInterface;
typedef struct Rational         Rational;
typedef struct Natural          Natural;

typedef struct Block            Block;
typedef struct Deck             Deck;
typedef struct List             List;
typedef struct Tree             Tree;
typedef struct Matrix           Matrix;
typedef struct Table            Table;
typedef struct HashTable        HashTable;

enum Status
{
    WHY_OK = 0, WHY_ERROR = -1, WHY_ELEMENT_EXISTS = 1, 
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

extern const Byte           ZERO_BYTE;

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

void*       MemExpand(void* memory, Uint size, Uint extra_size);
void*       MemExpandZero(void* memory, Uint size, Uint extra_size);
void        MemDestroy(void* ptr);
void*       MemZero(Uint size);

//type interface
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

//block
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
void        BlockInitFromArray(Block* block, const void* array);
void        BlockDestroy(Block* block);
void        BlockDestroyWRAP(void* block_pointer);
void        BlockDestroyNoMem(Block* block);
void*       BlockDestroyReturnContent(Block* block);
void*       BlockGetContent(const Block* block);
void*       BlockPointAt(const Block* block, Uint index);
Int         BlockCopyItems(Block* block, Uint index, const void* pointer, Uint n_items);
void        BlockGet(void* target, const Block* block, Uint index);
void        BlockSet(Block* block, Uint index, const void* item);
void        BlockSwap(Block* block, Uint j, Uint k);
Int         BlockWriteBytes(Block* block, Uint index, const Byte* bytes, Uint n_bytes);
Int         BlockCompare(const Block* block, Uint j, Uint k, Int (*compare)(const void* lhs, const void* rhs));
void        BlockMap(const Block* block, Uint index, Uint n_items, void (*function)(void *));
void        BlockFold(void* target, const Block* block, void (*fold)(void *, const void *, const void *));
void        BlockFoldNItems(void* target, const Block* block, Uint index, Uint n_items, 
                            void (*fold)(void *, const void *, const void *));
void*       BlockBinSearch(const Block* block, const void* item, Int (*compare)(const void *, const void *));
void*       BlockBinSearchRange(const Block* block, const void* item,
                            Int (*compare)(const void *, const void *), Uint left, Uint right);
void*       BlockSearchRange(const Block* block, const void* item,
                            Int (*compare)(const void *, const void *), Uint left, Uint right);
void        BlockReverseSlice(Block* block, Uint left, Uint right);
void        BlockReverse(Block* block);
bool        BlockPermuteLexicalSlice(Block* block, 
                            Int left, Int right, Int (*compare)(const void* , const void* ));
bool        BlockPermuteLexical(Block* block, Int (*compare)(const void* , const void *));

//deck
Deck*       DeckCreatePtr(void* (*copy)(const void *), void (*destroy)(void *));
Deck*       DeckCreateUint();
void        DeckDestroy(Deck* deck);
Deck*       DeckCopyStructure(const Deck* deck);
void        DeckDestroyWRAP(void* deck_pointer);
Uint        DeckNItems(const Deck* deck);
void        DeckGet(void* target, const Deck* deck, Uint index);
void        DeckSet(Deck* deck, Uint index, const void* item);
void*       DeckPointAt(const Deck* deck, Uint index);
Int         DeckPushBack(Deck* deck, const void* item);
Int         DeckPushFront(Deck* deck, const void* item);
void        DeckMap(Deck* deck, void (*function)(void *));
void        DeckReserve(Deck* deck, Uint n_items);
Int         DeckLast(void* target, const Deck* deck);
Int         DeckFirst(void* target, const Deck* deck);
void        DeckAppend(Deck* lhs, const Deck* rhs);
void        DeckReverse(Deck* deck);
Int         DeckCompare(const Deck* deck, Uint j, Uint k, Int (*compare)(const void *, const void *));
void*       DeckBinSearch(const Deck* deck, const void* item,
                            Int (*compare)(const void *, const void *));
void*       DeckSearchLinear(const Deck* deck, const void* item, Int (*compare)(const void *, const void *));
Deck*       DeckUnique(Deck* deck, Int (*compare)(const void *, const void *));
void        DeckFold(void* target, const Deck* deck, void (*fold)(void *, const void *, const void *));

//hash table
HashTable*  HashTableCreate(Uint capacity, void* (*copy)(const void *),
                            void (*destroy)(void *), Uint (*hash)(const void *));
Uint        HashTableNItems(const HashTable* table);
Uint        HashTableNCells(const HashTable* table);
void        HashTableDestroy(HashTable* table);
void        HashTableMapCell(HashTable* table, Uint n, void (*function)(void *));
void*       HashTableFind(const HashTable* table, const void* item,
                            Int (*compare)(const void *, const void *));
Int         HashTableInsert(HashTable* table, const void* item,
                            Int (*compare)(const void *, const void *));
HashTable*  HashDeck(const Deck* deck, Uint capacity, Uint (*hash)(const void *),
                    void* (*copy)(const void *), void (*destroy)(void *),
                    Int (*compare)(const void *, const void *));

//table
Int         TableAddRow(Table* table);
Table*      TableCreatePtr(void* (*copy)(const void *), void (*destroy)(void *));
Uint        TableNRows(const Table* table);
Int         TablePush(Table* table, Uint row, const void* item);
Int         TablePushLastRow(Table* table, const void* item);
void        TableMap(Table* table, void (*function)(void *));
void        TableMapRow(Table* table, Uint n, void (*function)(void *));
void        TableDestroy(Table* table);
void        TableDestroyWRAP(void* table_pointer);
Int         TableLast(void* target, const Table* table);
Int         TableFirst(void* target, const Table* table);
void*       TablePointAt(const Table* table, Uint row, Uint col);
Deck*       TablePointAtRow(const Table* table, Uint row);

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

//natural
void        NaturalInit(Natural* number, Uint n);
void        NaturalInitStr(Natural* number, const char* string, Uint length);
Natural*    NaturalCreateZero(Uint n_digits);
Natural*    NaturalCreate(const char* digit_string);
Natural*    NaturalCreateFromUint(Uint n);
void        NaturalDestroy(Natural* number);
Int         NaturalCompare(const Natural* lhs, const Natural* rhs);
Int         NaturalCompareWRAP(const void* lhs, const void* rhs);
Uint        NaturalNDIgits(const Natural* n);
char*       NaturalGetDigits(const Natural* number);
void        NaturalSet(Natural* number, const Natural* rhs);
void        NaturalAdd(Natural* target, const Natural* lhs, const Natural* rhs);
void        NaturalMult(Natural* target, const Natural* lhs, const Natural* rhs);
void        NaturalPower(Natural* target, const Natural* number, Uint exponent);

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
void        UintAdd(Uint* target, const Uint* lhs, const Uint* rhs);
void        UintAddWRAP(void* target, const void* lhs, const void* rhs);
void        UintMult(Uint* target, const Uint* lhs, const Uint* rhs);
void        UintMultWRAP(void* target, const void* lhs, const void* rhs);

void        IntInit(Int* target, Int value);

//compare
Int         CompareByte(const void* lhs, const void* rhs);
Int         CompareInt(const void* lhs, const void* rhs);
Int         CompareUint(const void* lhs, const void* rhs);
Int         ComapreFloat(const void* lhs, const void* rhs);
Int         CompareCstr(const void* lhs, const void* rhs);

//math
Int         MathUnitInit(Uint sieve_size);
void        MathUnitTerminate();
Uint        MathRandom(void);
Uint        MathRandomInRange(Uint n);
bool        MathIsPrime(Uint n);
Deck*       MathGetNPrimes(Uint n);
Uint        MathGCD(Uint a, Uint b);
Int         MathGCDInt(Int a, Int b);
Uint        MathGCDArray(const Uint* array, Uint size);
Uint        MathLCM(Uint a, Uint b);
Uint        MathLCMArray(const Uint* array, Uint size);
Uint        MathFib(Uint n);
Deck*       MathFactor(Uint n);
Deck*       MathComputeDivisors(Uint n);
Block*      MathSieve(Uint size);
Uint        MathGetNthPrime(Uint n);
Block*      MathGetSieve();
Uint        MathFactorial(Uint n);
Uint        MathCountDigits(Uint n);

//matrix
Matrix*     MatrixCreateFloat(Uint n_rows, Uint n_cols);
Matrix*     MatrixCreateRational(Uint n_rows, Uint n_cols);
Matrix*     MatrixCreateUint(Uint n_rows, Uint n_cols);
Matrix*     MatrixCreateRationalFromTable(const Table* table);
Matrix*     MatrixCreateUintFromTable(const Table* table);
Matrix*     MatrixCreateUintFromTableWithParser(const Table* table, Int (*parser)(void *, const char *));
void        MatrixDestroy(Matrix* matrix);
void        MatrixDestroyWRAP(void* matrix_pointer);
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
void        MatrixDiagonalize(Matrix* matrix);
//these should be made private
void        MatrixSwapRows(Matrix* matrix, Uint j, Uint k);
void        MatrixScaleRow(Matrix* matrix, Uint row, const void* value);
void        MatrixAddRows(Matrix* matrix, Uint j, Uint k);
void        MatrixAddScaledRows(Matrix* matrix, Uint target, Uint source, const void* factor);
Int         MatrixFindPivotRow(const Matrix* matrix);
void        MatrixRowEliminate(Matrix* matrix, Uint pivot_row, Uint target_row);
void        MatrixEchelonForm(Matrix* matrix);
void        MatrixEliminateUp(Matrix* matrix);

//parse
bool        IsDigit(char c);
bool        IsAlpha(char c);
bool        IsSpace(char c);
Int         ParseUint32(uint32_t* target, const char* string, Uint length);
Int         ParseUint(Uint* target, const char* string);
Int         ParseInt(Int* target, const char* string);
Int         ParseRational(Rational* target, const char* string);
Int         ParseDigitSequence(Uint* target, const char* string);
Int         ParseDigitSequenceWRAP(void* target, const char* string);
Int         ParseUintWRAP(void* target, const char* string);
Int         ParseIntWRAP(void* target, const char* string);
Int         ParseRationalWRAP(void* target, const char* string);
Int         ParseTable(Table* table, const char* string, char table_sep_left, char table_sep_right,
                        char row_sep_left, char row_sep_right, char col_sep);

//string
char*       StringSubstring(const char* string, Uint length);
char*       StringNCopy(const char* string, Uint n);
Int         StringFindC(const char* string, char c);
Deck*       StringSplitLength(const char* string, char separator, Uint length);
Deck*       StringSplit(const char* string, char separator);
Deck*       StringSplitDestructive(char* string, char separator);
Deck*       StringSplitLengthDestructive(char* string, char separator, Uint length);
Deck*       StringSplitStr(char* string, const char* substring);
Byte*       StringSplice(const Deck* strings);
Byte*       StringSplitSplice(char* string, const char* substring);
Deck*       StringCut(const char* string, Uint cut_size);
Deck*       StringCutFromEnd(const char* string, Uint cut_size);
char*       StringConcat(const char* lhs, const char* rhs);
char*       StringPrepend(const char* string, char c);
char*       StringStripFront(const char* string, char c);
void        StringStripBackDestructive(char* string, char c);
char*       StringStrip(const char* string, char front, char back);
void        StringReverseLength(char* string, Uint length);
void        StringReverse(char* string);
char*       StringCreateReversed(const char* string);
Uint        StringHash(const char* string);
Uint        StringHashLength(const char* string, Uint length);
Uint        StringHashWRAP(const void* string);

//input
Byte*       ReadFile(const char* name);
Deck*       ReadFileAllLines(const char* name);
Deck*       ReadFileAllLines2(const char* name);
Byte*       ReadFileSplitSplice(const char* name, const char* substring);

//print
void        PrintCstr(const char* str);
void        PrintCstrWRAP(const void* str_pointer, const void* sep);
void        PrintCstrN(const void* str);
void        PrintCstrS(const void* str);
void        PrintCstrP(const void* str);
void        PrintRational(const Rational* p);
void        PrintRationalWRAP(const void* p_pointer, const void* sep);
void        PrintRationalN(const void* p);
void        PrintRationalS(const void* p);
void        PrintRationalP(const void* p);
void        PrintUint(const Uint n);
void        PrintUintWRAP(const void* n_pointer, const void* sep);
void        PrintUintN(const void* n);
void        PrintUintS(const void* n);
void        PrintByte(Byte b);
void        PrintChar(char c);
void        PrintCharWRAP(const void* c, const void* sep);
void        PrintCharS(const void* c);
void        PrintByteWRAP(const void* byte_pointer, const void* sep);
void        PrintByteN(const void* b);
void        PrintByteS(const void* byte);
void        PrintInt(const Int n);
void        PrintIntWRAP(const void* n_pointer, const void* sep);
void        PrintIntN(const void* n);
void        PrintIntS(const void* n);
void        PrintFloat(const Float x);
void        PrintFloatWRAP(const void* x_pointer, const void* sep);
void        PrintFloatS(const void* x);
void        PrintTimeDiff(const void* start, const void* end);
void        PrintNBits(Uint number, Uint n_bits);
void        PrintBits(Uint n);
void        PrintBlock(const Block* block, void (*print)(const void *));
void        PrintBlockWRAP(const void* block, void (*print)(const void *), const void *sep);
void        PrintBlockN(const void* block, void (*print)(const void *));
void        PrintMatrix(const Matrix* matrix, void (*print)(const void* ));
void        PrintDeck(const Deck* deck, void (*print)(const void *));
void        PrintTable(const Table* table, void (*print)(const void* ));
void        PrintNatural(const Natural* natural);
void        PrintNaturalWRAP(const void* n, const char* sep);
void        PrintNaturalN(const void* n);
void        PrintHashTable(const HashTable* table, void (*print)(const void *));

#endif