#ifndef __FIFO_H__
#define __FIFO_H__

#include "types.h"

#define FIFO_DATA_LEN (SIZE_T) 32

typedef struct {
    U8_T data[FIFO_DATA_LEN];
    /* Data length */
    SIZE_T len;
} FIFO_ENTRY_T;


typedef struct {
    /* Underlying buffer */
    FIFO_ENTRY_T* buf;
    /* Underlying buffer size */
    SIZE_T size;
    /* Read pointer */
    U8_T head;
    /* Write pointer */
    U8_T tail;
    /* Number of entries */
    SIZE_T n;
} FIFO_T;


typedef enum {
    FIFO_EMPTY,
    FIFO_FULL,
    FIFO_OK
} FIFO_STATUS_T;


void fifo_q_init(FIFO_T* q, FIFO_ENTRY_T* buf, SIZE_T size);
FIFO_STATUS_T fifo_q_add(FIFO_T* q, U8_T* src, SIZE_T len);
FIFO_STATUS_T fifo_q_remove(FIFO_T* q, U8_T* dst, SIZE_T* len);
SIZE_T fifo_q_len(FIFO_T* q);

#endif
