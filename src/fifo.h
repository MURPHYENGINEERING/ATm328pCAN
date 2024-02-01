#ifndef __FIFO_H__
#define __FIFO_H__

#include "types.h"

/** Maximum length of data in bytes in any FIFO queue element. */
#define FIFO_DATA_LEN (SIZE_T) 32

/** Represents a single element in a FIFO queue. */
typedef struct {
    /** Data associated with this FIFO queue element. */
    U8_T data[FIFO_DATA_LEN];
    /** Length of data in the `data` array in bytes. */
    SIZE_T len;
} FIFO_ENTRY_T;


/** A FIFO (First-In-First-Out) queue. */
typedef struct {
    /** Underlying buffer storing the elements of this queue. */
    FIFO_ENTRY_T* buf;
    /** Maximum number of elements that can be stored in the underlying buffer. */
    SIZE_T size;
    /** Read index, in elements, into the underlying buffer. */
    U8_T head;
    /** Write index, in elements, into the underlying buffer. */
    U8_T tail;
    /** Number of elements currently stored in the underlying buffer. */
    SIZE_T n;
} FIFO_T;


/** Represents the status of a FIFO queue operation (e.g., adding an element
 * may return `FIFO_OK` or `FIFO_FULL`. */
typedef enum {
    /** The FIFO queue is empty and no elements can be removed from it. */
    FIFO_EMPTY,
    /** The FIFO queue is full and no elements can be added to it. */
    FIFO_FULL,
    /** The FIFO queue operation was successful. */
    FIFO_OK
} FIFO_STATUS_T;


void fifo_q_init(FIFO_T* q, FIFO_ENTRY_T* buf, SIZE_T size);
FIFO_STATUS_T fifo_q_add(FIFO_T* q, U8_T* src, SIZE_T len);
FIFO_STATUS_T fifo_q_remove(FIFO_T* q, U8_T* dst, SIZE_T* len);
SIZE_T fifo_q_len(FIFO_T* q);

#endif
