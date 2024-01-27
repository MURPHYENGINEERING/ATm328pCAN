#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"


void memcpy_by_U8(U8_T* dst, U8_T* src, U32_T len);

S32_T memcmp_by_U8(U8_T* lhs, U8_T* rhs, U32_T len);


#endif