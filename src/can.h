#ifndef __CAN_H__
#define __CAN_H__

#include "fifo.h"


void can_init(void);

FIFO_STATUS_T can_tx_q_add(U8_T* buf, SIZE_T len);
FIFO_STATUS_T can_rx_q_remove(U8_T* buf, SIZE_T* len);
SIZE_T can_rx_q_len(void);

void task_can_tx(void);
void task_can_rx(void);

#endif