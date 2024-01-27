#include "memory.h"


void memcpy_by_U8(U8_T* dst, U8_T* src, U32_T len)
{
    U32_T i;

    for (i = 0; i < len; ++i) {
        *dst = *src;
        ++dst;
        ++src;
    }
}


S32_T memcmp_by_U8(U8_T* lhs, U8_T* rhs, U32_T len)
{
    U32_T i;
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