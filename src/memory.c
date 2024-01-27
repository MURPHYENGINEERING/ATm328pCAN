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
    result = *lhs - *rhs;

    while ( (0 == result) && (i < len) ) {
        ++lhs;
        ++rhs;
        ++i;
        result = *lhs - *rhs;
    }

    return result;
}