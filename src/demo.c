#include "types.h"
#include "demo.h"
#include "fifo.h"
#include "can.h"
#include "memory.h"
#include "string.h"
#include "dsc.h"


void task_demo_tx(void)
{
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T len;
    FIFO_STATUS_T status;

    memcpy_by_U8(buf, (U8_T*) "Hello, world!", (SIZE_T) 13);
    len = strnlen_by_U8((U8_T*) "Hello, world!", FIFO_DATA_LEN);

    status = can_tx_q_add(buf, len);

    if (FIFO_OK == status) {
    } else {
        /* TODO: Buffer overflow, report fault */
    }
}


void task_demo_rx(void)
{
    SIZE_T n_pending_msgs;
    SIZE_T i;
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T len;

    n_pending_msgs = can_rx_q_len();

    for (i = 0; i < n_pending_msgs; ++i) {
        status = can_rx_q_remove(buf, &len);

        if (FIFO_OK == status) {
            /* TODO: What do we do with received messages? */

        } else {
            /* TODO: Shouldn't underflow, report software error */
        }
    }
}