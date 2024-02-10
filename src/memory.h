#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

SIZE_T memcpy(void* dst, void* src, SIZE_T len);
void memset(void* dst, U8_T value, SIZE_T len);
S8_T memcmp(void* lhs, void* rhs, SIZE_T len);

#pragma GCC diagnostic pop


/* Noinline because it needs to be a function call (it returns the PC that's on
 * the stack for return). */
__attribute__((__noinline__)) U16_T get_pc(void);


#endif