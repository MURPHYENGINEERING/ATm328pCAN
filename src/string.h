#ifndef __STRING_H___
#define __STRING_H___

#include "types.h"

SIZE_T strnlen(S8_T* s, SIZE_T max);

void str_reverse(S8_T* s);

SIZE_T itoa(S8_T* s, U32_T n);


#endif