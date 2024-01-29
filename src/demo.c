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

    status = can_q_add(buf, len);

    if (FIFO_OK == status) {
    } else {
        /* Buffer overrun, report fault */
    }
}