#include "can.h"


void task_can_rx(void)
{
    /* TODO: read a message */
}


void task_can_tx(void)
{
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T n_pending_msgs;
    SIZE_T len;
    SIZE_T i;

    n_pending_msgs = fifo_q_len(&g_can_tx_q);

    for (i = 0; i < n_pending_msgs; ++i) {
        status = fifo_q_remove(&g_can_tx_q, buf, &len);

        if (FIFO_OK == status) {
            can_tx(buf, len);
        } else {
            /* Shouldn't underflow, report software fault */
        }
    }
}