#include "string.h"


SIZE_T strnlen_by_U8(U8_T* buf, SIZE_T max)
{
    SIZE_T i;

    for (i = 0; (buf[i] != 0) && (i < max); ++i) {
    }

    return i;
}