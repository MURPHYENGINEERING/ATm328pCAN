#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"


void memcpy_by_U8(U8_T* dst, U8_T* src, SIZE_T len);
void memset_by_U8(U8_T* dst, U8_T value, SIZE_T len);

S32_T memcmp_by_U8(U8_T* lhs, U8_T* rhs, SIZE_T len);

__attribute__((__noinline__)) U16_T get_pc(void);


#endif