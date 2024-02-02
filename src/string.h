#ifndef __STRING_H___
#define __STRING_H___

#include "types.h"

SIZE_T strnlen_by_U8(U8_T* s, SIZE_T max);

void str_reverse(U8_T* s);

SIZE_T itoa(U8_T* s, U32_T n);


#endif