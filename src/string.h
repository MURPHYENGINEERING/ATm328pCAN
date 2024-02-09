#ifndef __STRING_H___
#define __STRING_H___

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
SIZE_T strnlen(CSTR_T s, SIZE_T max);
SIZE_T strncpy(CSTR_T s_dst, CSTR_T s_src, SIZE_T max);
CHAR_T strncmp(CSTR_T s_lhs, CSTR_T s_rhs, SIZE_T max);
#pragma GCC diagnostic pop

void strreverse(CSTR_T s);

SIZE_T itoa(CSTR_T s, U32_T n);


#endif