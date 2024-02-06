#include "fifo.h"
#include "memory.h"


/*******************************************************************************
 * Initialize the given FIFO queue to point to the given underlying buffer of
 * the given size.
 * \param[in] q     The queue to be initialized.
 * \param[in] buf   The underlying buffer to be pointed to by the FIFO queue.
 * \param[in] size  The length of the underlying buffer to be pointed to.
 ******************************************************************************/
void fifo_q_init(FIFO_T* q, FIFO_ENTRY_T* buf, SIZE_T size)
{
    q->buf = buf;
    q->size = size;
    q->head = 0u;
    q->tail = 0u;
    q->n = (SIZE_T) 0u;

    memset(buf, (U8_T) 0u, (SIZE_T)( size * sizeof(FIFO_ENTRY_T) ));
}


/*******************************************************************************
 * Add the given data of the given length in bytes to the given queue.
 * \param[in] q     The queue to be added to.
 * \param[in] src   The byte array to be written to the new queue element.
 * \param[in] len   The length of the byte array to be written to the queue.
 * \retval FIFO_OK      If the element was added to the queue.
 * \retval FIFO_FULL    If the queue is full and no element was added.
 ******************************************************************************/
FIFO_STATUS_T fifo_q_add(FIFO_T* q, U8_T* src, SIZE_T len)
{
    FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if (q->n == q->size) {
        status = FIFO_FULL;
    } else {
        p_fifo_entry = &q->buf[q->tail];
        memcpy(p_fifo_entry->data, src, len);
        p_fifo_entry->len = len;

        q->tail = (SIZE_T)( (q->tail + 1u) % q->size );
        ++q->n;

        status = FIFO_OK;
    }

    return status;
}


/*******************************************************************************
 * Remove an element from the given queue, returning its data into the given
 * buffer, and returning its length into the given pointer.
 * \param[in] q     The queue to be read from, removing an element from its end.
 * \param[out] dst  The buffer that the removed element will be written into.
 * \param[out] len  The pointer into which the length of the read data will be 
 *                  placed.
 ******************************************************************************/
FIFO_STATUS_T fifo_q_remove(FIFO_T* q, U8_T* dst, SIZE_T* len)
{
    FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if ((SIZE_T) 0u == q->n) {
        status =  FIFO_EMPTY;
    } else {
        p_fifo_entry = &q->buf[q->head];
        memcpy(dst, p_fifo_entry->data, p_fifo_entry->len);
        *len = p_fifo_entry->len;

        q->head = (SIZE_T)( (q->head + 1u) % q->size );
        --q->n;

        status = FIFO_OK;
    }

    return status;
}


/*******************************************************************************
 * Get the length of the given FIFO queue.
 * \param[in] q The FIFO queue whose length will be returned.
 * \return The length of the given queue.
 ******************************************************************************/
SIZE_T fifo_q_len(FIFO_T* q)
{
    return q->n;
}