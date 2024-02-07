#ifndef __STRING_H___
#define __STRING_H___

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
SIZE_T strnlen(CSTR_T s, SIZE_T max);
SIZE_T strncpy(CSTR_T dst, CSTR_T src, SIZE_T max);
CHAR_T strncmp(CSTR_T lhs, CSTR_T rhs, SIZE_T max);
#pragma GCC diagnostic pop

void strreverse(CSTR_T s);

SIZE_T itoa(CSTR_T s, U32_T n);


#endif