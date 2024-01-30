#include "memory.h"


void memcpy_by_U8(U8_T* dst, U8_T* src, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        *dst = *src;
        ++dst;
        ++src;
    }
}


void memset_by_U8(U8_T* dst, U8_T value, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        *dst = value;
        ++dst;
    }
}


S32_T memcmp_by_U8(U8_T* lhs, U8_T* rhs, SIZE_T len)
{
    SIZE_T i;
    S32_T result;

    i = 0;
    result = (S32_T) *lhs - (S32_T) *rhs;

    while ( ((S32_T) 0 == result) && (i < len) ) {
        result = (S32_T) *lhs - (S32_T) *rhs;
        ++lhs;
        ++rhs;
        ++i;
    }

    return result;
}


U32_T get_pc(void) 
{
    return (U32_T) __builtin_return_address(0);
}