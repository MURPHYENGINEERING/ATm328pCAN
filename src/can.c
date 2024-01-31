#include "types.h"
#include "can.h"
#include "memory.h"
#include "fai.h"


/******************************************************************************/
/* CAN Queues */
CAN_FIFO_ENTRY_T g_can_tx_q_buf[CAN_FIFO_TX_SIZE];
CAN_FIFO_ENTRY_T g_can_rx_q_buf[CAN_FIFO_RX_SIZE];

CAN_FIFO_T g_can_tx_q;
CAN_FIFO_T g_can_rx_q;
/******************************************************************************/


void can_init(void)
{
    can_fifo_q_init(&g_can_tx_q, g_can_tx_q_buf, CAN_FIFO_TX_SIZE);
    can_fifo_q_init(&g_can_rx_q, g_can_rx_q_buf, CAN_FIFO_RX_SIZE);

    can_init_hardware();
}


void task_can_rx(void)
{
    /* TODO: read a message */
}


void task_can_tx(void)
{
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T n_pending_msgs;
    CAN_IDENT_T identifier;
    SIZE_T len;
    SIZE_T i;

    n_pending_msgs = fifo_q_len(&g_can_tx_q);

    for (i = 0; i < n_pending_msgs; ++i) {
        status = can_fifo_q_remove(&g_can_tx_q, &identifier, buf, &len);

        if (FIFO_OK == status) {
            can_tx(identifier, buf, len);
        } else {
            fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        }
    }
}


FIFO_STATUS_T can_tx_q_add(CAN_IDENT_T identifier, U8_T* buf, SIZE_T len)
{
    FIFO_STATUS_T status;

    status = can_fifo_q_add(&g_can_tx_q, identifier, buf, len);

    return status;
}


FIFO_STATUS_T can_rx_q_remove(CAN_IDENT_T* identifier, U8_T* buf, SIZE_T* len)
{
    FIFO_STATUS_T status;

    status = can_fifo_q_remove(&g_can_rx_q, identifier, buf, len);

    return status;
}


SIZE_T can_rx_q_len(void)
{
    return can_fifo_q_len(&g_can_rx_q);
}


void can_fifo_q_init(CAN_FIFO_T* q, CAN_FIFO_ENTRY_T* buf, SIZE_T size)
{
    q->buf = buf;
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->n = (SIZE_T) 0;

    memset_by_U8(
        (U8_T*)(void*) buf, 
        (U8_T) 0, 
        (SIZE_T)( size * sizeof(CAN_FIFO_ENTRY_T) )
    );
}


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


FIFO_STATUS_T can_fifo_q_remove(
    CAN_FIFO_T* q, 
    CAN_IDENT_T* identifier, 
    U8_T* dst, 
    SIZE_T* len
)
{
    FIFO_STATUS_T status;
    CAN_FIFO_ENTRY_T* p_fifo_entry;

    if ((SIZE_T) 0 == q->n) {
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


SIZE_T can_fifo_q_len(CAN_FIFO_T* q)
{
    return q->n;
}