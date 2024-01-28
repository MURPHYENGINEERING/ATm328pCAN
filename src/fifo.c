#include "fifo.h"
#include "memory.h"


void fifo_q_init(FIFO_T* q, FIFO_ENTRY_T* buf, U8_T size)
{
    q->buf = buf;
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->n = 0;
}


FIFO_STATUS_T fifo_q_add(FIFO_T* q, U8_T* src, U32_T len)
{
    FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if (q->n == q->size) {
        status = FIFO_FULL;
    } else {
        p_fifo_entry = &q->buf[q->tail];
        memcpy_by_U8(p_fifo_entry->data, src, len);
        p_fifo_entry->len = len;

        q->tail = (q->tail + 1) % q->size;
        ++q->n;

        status = FIFO_OK;
    }

    return status;
}


FIFO_STATUS_T fifo_q_remove(FIFO_T* q, U8_T* dst, U32_T* len)
{
    FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if (0 == q->n) {
        status =  FIFO_EMPTY;
    } else {
        p_fifo_entry = &q->buf[q->head];
        memcpy_by_U8(dst, p_fifo_entry->data, p_fifo_entry->len);
        *len = p_fifo_entry->len;

        q->head = (q->head + 1) % q->size;
        --q->n;

        status = FIFO_OK;
    }

    return status;
}
