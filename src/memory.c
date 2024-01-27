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
