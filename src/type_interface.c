#include "type_interface.h"
#include "rational.h"
#include "macro.h"

const TypeInterface PtrInterface = {GetPtr, SetPtr, SwapPtr, sizeof(void *)};
const TypeInterface ByteInterface = {GetByte, SetByte, SwapByte, sizeof(Byte)};
const TypeInterface IntInterface = {GetInt, SetInt, SwapInt, sizeof(Int)};
const TypeInterface UintInterface = {GetUint, SetUint, SwapUint, sizeof(Uint)};
const TypeInterface FloatInterface = {GetFloat, SetFloat, SwapFloat, sizeof(Float)};
const TypeInterface RationalInterface = {GetRational, SetRational, SwapRational, sizeof(Rational)};
const TypeInterface ComplexInterface = {GetComplex, SetComplex, SwapComplex, sizeof(Complex)};

void GetPtr(void* target, const void* memory, Uint index)
{
    *(void **)target = ((void **)memory)[index];
}

void SetPtr(void* memory, Uint index, const void* ptr)
{
    ((const void **)memory)[index] = ptr;
}

void SwapPtr(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, void *);
}

void GetByte(void* target, const void* memory, Uint index)
{
    TARGET_SET(target, memory, index, Byte);
}

void SetByte(void* memory, Uint index, const void* ptr)
{
    MEM_SET_FP(memory, index, ptr, Byte);
}

void SwapByte(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, Byte);
}

void GetInt(void* target, const void* memory, Uint index)
{
    TARGET_SET(target, memory, index, Int);
}

void SetInt(void* memory, Uint index, const void* ptr)
{
    MEM_SET_FP(memory, index, ptr, Int);
}

void SwapInt(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, Int);
}

void GetUint(void* target, const void* memory, Uint index)
{
    TARGET_SET(target, memory, index, Uint);
}

void SetUint(void* memory, Uint index, const void* ptr)
{
    MEM_SET_FP(memory, index, ptr, Uint);
}

void SwapUint(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, Uint);
}

void GetFloat(void* target, const void* memory, Uint index)
{
    TARGET_SET(target, memory, index, Float);
}

void SetFloat(void* memory, Uint index, const void* ptr)
{
    MEM_SET_FP(memory, index, ptr, Float);
}

void SwapFloat(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, Float);
}

void GetRational(void* target, const void* memory, Uint index)
{
    TARGET_SET(target, memory, index, Rational);
}

void SetRational(void* memory, Uint index, const void* ptr)
{
    MEM_SET_FP(memory, index, ptr, Rational);
}

void SwapRational(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, Rational);
}

void GetComplex(void* target, const void* memory, Uint index)
{
    TARGET_SET(target, memory, index, Complex);
}

void SetComplex(void* memory, Uint index, const void* ptr)
{
    MEM_SET_FP(memory, index, ptr, Complex);
}

void SwapComplex(void* memory, Uint j, Uint k)
{
    MEM_SWAP(memory, j, k, Complex);
}
