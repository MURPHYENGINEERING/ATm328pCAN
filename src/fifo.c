#include "fifo.h"
#include "memory.h"


void fifo_q_init(FIFO_T* q, FIFO_ENTRY_T* buf, SIZE_T size)
{
    q->buf = buf;
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->n = (SIZE_T) 0;

    memset_by_U8((U8_T*)(void*) buf, (U8_T) 0, (SIZE_T)( size * sizeof(FIFO_ENTRY_T) ));
}


FIFO_STATUS_T fifo_q_add(FIFO_T* q, U8_T* src, SIZE_T len)
{
    FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if (q->n == q->size) {
        status = FIFO_FULL;
    } else {
        p_fifo_entry = &q->buf[q->tail];
        memcpy_by_U8(p_fifo_entry->data, src, len);
        p_fifo_entry->len = len;

        q->tail = (SIZE_T)( (q->tail + 1u) % q->size );
        ++q->n;

        status = FIFO_OK;
    }

    return status;
}


FIFO_STATUS_T fifo_q_remove(FIFO_T* q, U8_T* dst, SIZE_T* len)
{
    FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if ((SIZE_T) 0 == q->n) {
        status =  FIFO_EMPTY;
    } else {
        p_fifo_entry = &q->buf[q->head];
        memcpy_by_U8(dst, p_fifo_entry->data, p_fifo_entry->len);
        *len = p_fifo_entry->len;

        q->head = (SIZE_T)( (q->head + 1u) % q->size );
        --q->n;

        status = FIFO_OK;
    }

    return status;
}


SIZE_T fifo_q_len(FIFO_T* q)
{
    return q->n;
}