#include "types.h"
#include "can.h"
#include "memory.h"
#include "fai.h"
#include "fifo.h"


/* CAN Queues */
CAN_FIFO_ENTRY_T g_can_tx_q_buf[CAN_FIFO_TX_SIZE];
CAN_FIFO_ENTRY_T g_can_rx_q_buf[CAN_FIFO_RX_SIZE];

CAN_FIFO_T g_can_tx_q;
CAN_FIFO_T g_can_rx_q;


/*******************************************************************************
 * Initialize the CAN software FIFOs and hardware.
 ******************************************************************************/
void can_init(void)
{
    can_fifo_q_init(&g_can_tx_q, g_can_tx_q_buf, CAN_FIFO_TX_SIZE);
    can_fifo_q_init(&g_can_rx_q, g_can_rx_q_buf, CAN_FIFO_RX_SIZE);

    /* Start SPI bus */
    spi_init(can_get_spi_cfg());

    /* Start MCP2515 */
    can_init_hardware();
}


/*******************************************************************************
 * Perform the periodic CAN receive task, receiving messages from the CAN 
 * hardware and adding them to the software FIFO.
 ******************************************************************************/
void task_can_rx(void)
{
    /* TODO: read a message */
}


/*******************************************************************************
 * Perform the periodic CAN transmit task, removing messages from the TX software
 * FIFO and transmitting them to the CAN hardware driver.
 ******************************************************************************/
void task_can_tx(void)
{
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T n_pending_msgs;
    CAN_IDENT_T identifier;
    SIZE_T len;
    SIZE_T i;

    n_pending_msgs = can_fifo_q_len(&g_can_tx_q);

    for (i = 0; i < n_pending_msgs; ++i) {
        status = can_fifo_q_remove(&g_can_tx_q, &identifier, buf, &len);

        if (FIFO_OK == status) {
            can_tx(identifier, buf, len);
        } else {
            fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        }
    }
}


/*******************************************************************************
 * Add an element to the CAN TX FIFO queue, if it's not full.
 * \param[in] identifier    The CAN message identifier.
 * \param[in] buf           Pointer to the CAN message data buffer.
 * \param[in] len           Length in bytes of the CAN message data.
 * \retval FIFO_OK      The CAN message was added to the TX FIFO queue.
 * \retval FIFO_FULL    The TX FIFO queue is full and no message was added.
 ******************************************************************************/ 
FIFO_STATUS_T can_tx_q_add(CAN_IDENT_T identifier, U8_T* buf, SIZE_T len)
{
    FIFO_STATUS_T status;

    status = can_fifo_q_add(&g_can_tx_q, identifier, buf, len);

    return status;
}


/*******************************************************************************
 * Remove an element from the CAN RX FIFO queue and return it, if it's not empty.
 * \param[out] identifier   The CAN message identifier will be returned in this
 *                          pointer.
 * \param[out] buf          The CAN message data will be returned into this buffer.
 * \param[out] len          The length in bytes of the CAN message data will be
 *                          returned in this pointer.
 * \retval FIFO_OK      A CAN message was dequeued and returned.
 * \retval FIFO_EMPTY   The queue is empty and no message was returned. The state
 *                      of the argument pointers is unchanged.
 ******************************************************************************/ 
FIFO_STATUS_T can_rx_q_remove(CAN_IDENT_T* identifier, U8_T* buf, SIZE_T* len)
{
    FIFO_STATUS_T status;

    status = can_fifo_q_remove(&g_can_rx_q, identifier, buf, len);

    return status;
}


/*******************************************************************************
 * Get the number of elements in the CAN RX FIFO queue.
 * \return The number of elements in the CAN RX FIFO queue.
 ******************************************************************************/ 
SIZE_T can_rx_q_len(void)
{
    return can_fifo_q_len(&g_can_rx_q);
}


/*******************************************************************************
 * Initialize the given CAN FIFO queue to zero/empty.
 * \param[in] q     The given queue to be initialized.
 * \param[in] buf   Pointer to the underlying buffer for this queue. The queue
 *                  data will be stored in this buffer. The buffer is treated as
 *                  a ring, with reads and writes constantly advancing, wrapping
 *                  at `size` bytes.
 ******************************************************************************/ 
void can_fifo_q_init(CAN_FIFO_T* q, CAN_FIFO_ENTRY_T* buf, SIZE_T size)
{
    q->buf = buf;
    q->size = size;
    q->head = 0u;
    q->tail = 0u;
    q->n = (SIZE_T) 0u;

    memset_by_U8(
        (U8_T*)(void*) buf, 
        (U8_T) 0u, 
        (SIZE_T)( size * sizeof(CAN_FIFO_ENTRY_T) )
    );
}


/*******************************************************************************
 * Add an element to the given CAN FIFO queue.
 * \param[in] q             The given CAN FIFO queue to add an element to.
 * \param[in] identifier    The 11-bit CAN message identifier.
 * \param[in] src           Pointer to the CAN message data buffer.
 * \param[in] len           Length in bytes of data in the CAN message data buffer.
 *
 * \retval      FIFO_OK     The element was added to the queue.
 * \retval      FIFO_FULL   The queue is full and no element was added to it.
 ******************************************************************************/ 
FIFO_STATUS_T can_fifo_q_add(
    CAN_FIFO_T* q, 
    CAN_IDENT_T identifier, 
    U8_T* src, 
    SIZE_T len
)
{
    FIFO_STATUS_T status;
    CAN_FIFO_ENTRY_T* p_fifo_entry;

    if (q->n == q->size) {
        status = FIFO_FULL;
    } else {
        p_fifo_entry = &q->buf[q->tail];
        p_fifo_entry->identifier = identifier;
        memcpy_by_U8(p_fifo_entry->data, src, len);
        p_fifo_entry->len = len;

        q->tail = (SIZE_T)( (q->tail + 1u) % q->size );
        ++q->n;

        status = FIFO_OK;
    }

    return status;
}


/*******************************************************************************
 * Remove an element from the given CAN FIFO queue, returning it.
 * \param[in] q             The given CAN FIFO queue to remove an element from.
 * \param[out] identifier   The 11-bit CAN message identifier will be returned 
 *                          into this pointer.
 * \param[out] dst          The CAN message data will be returned into this buffer.
 * \param[out] len          The length of the CAN message data in bytes will be
 *                          returned into this pointer.
 * \retval                  FIFO_OK     The element was removed and returned.
 * \retval                  FIFO_EMPTY  There are no elements to remove.
 ******************************************************************************/ 
FIFO_STATUS_T can_fifo_q_remove(
    CAN_FIFO_T* q, 
    CAN_IDENT_T* identifier, 
    U8_T* dst, 
    SIZE_T* len
)
{
    FIFO_STATUS_T status;
    CAN_FIFO_ENTRY_T* p_fifo_entry;

    if ((SIZE_T) 0u == q->n) {
        status =  FIFO_EMPTY;
    } else {
        p_fifo_entry = &q->buf[q->head];
        *identifier = p_fifo_entry->identifier;
        memcpy_by_U8(dst, p_fifo_entry->data, p_fifo_entry->len);
        *len = p_fifo_entry->len;

        q->head = (SIZE_T)( (q->head + 1u) % q->size );
        --q->n;

        status = FIFO_OK;
    }

    return status;
}


/*******************************************************************************
 * Get the number of elements in the given CAN FIFO queue.
 * \param[in] q     The FIFO queue of CAN messages.
 * \return          The number of elements in the given CAN FIFO queue.
 ******************************************************************************/ 
SIZE_T can_fifo_q_len(CAN_FIFO_T* q)
{
    return q->n;
}