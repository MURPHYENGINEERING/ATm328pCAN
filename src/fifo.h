#ifndef __FIFO_H__
#define __FIFO_H__

#include "types.h"

#define FIFO_DATA_LEN 256

typedef struct {
    U8_T data[FIFO_DATA_LEN];
    U32_T len;
} FIFO_ENTRY_T;


typedef struct {
    FIFO_ENTRY_T* buf;
    U8_T len;
    U8_T head;
    U8_T tail;
    U8_T n;
} FIFO_T;


typedef enum {
    FIFO_EMPTY,
    FIFO_FULL,
    FIFO_OK
} FIFO_STATUS_T;


FIFO_STATUS_T fifo_q_add(FIFO_T* q, U8_T* src, U32_T len);
FIFO_STATUS_T fifo_q_remove(FIFO_T* q, U8_T* dst, U32_T* len);

#endif
