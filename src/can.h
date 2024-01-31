#ifndef __CAN_H__
#define __CAN_H__

#include "fifo.h"


#define CAN_FIFO_TX_SIZE (SIZE_T) 4u
#define CAN_FIFO_RX_SIZE (SIZE_T) 4u

#define CAN_FIFO_DATA_LEN (SIZE_T) 8

typedef struct {
    U16_T identifier;
    U8_T data[FIFO_DATA_LEN];
    /* Data length */
    SIZE_T len;
} FIFO_ENTRY_T;

typedef struct
{
    /* Underlying buffer */
    CAN_FIFO_ENTRY_T* buf;
    /* Underlying buffer size */
    SIZE_T size;
    /* Read pointer */
    U8_T head;
    /* Write pointer */
    U8_T tail;
    /* Number of entries */
    SIZE_T n;
} CAN_FIFO_T;


/******************************************************************************/
/* CAN Queues */
extern CAN_FIFO_ENTRY_T g_can_tx_q_buf[CAN_FIFO_TX_SIZE];
extern CAN_FIFO_ENTRY_T g_can_rx_q_buf[CAN_FIFO_RX_SIZE];

extern CAN_FIFO_T g_can_tx_q;
extern CAN_FIFO_T g_can_rx_q;
/******************************************************************************/


void can_init(void);
void can_init_hardware(void);

void can_fifo_q_init(CAN_FIFO_T* q, CAN_FIFO_ENTRY_T* buf, SIZE_T size);

FIFO_STATUS_T can_fifo_q_add(
    CAN_FIFO_T* q, 
    U16_T identifier, 
    U8_T* src, 
    SIZE_T len
);

FIFO_STATUS_T can_fifo_q_remove(
    CAN_FIFO_T* q, 
    U16_T* identifier, 
    U8_T* dst, 
    SIZE_T* len
);

SIZE_T can_fifo_q_len(CAN_FIFO_T* q);

FIFO_STATUS_T can_tx_q_add(U16_T identifier, U8_T* buf, SIZE_T len);
FIFO_STATUS_T can_rx_q_remove(U16_T* identifier, U8_T* buf, SIZE_T* len);
SIZE_T can_rx_q_len(void);

void task_can_tx(void);
void task_can_rx(void);


#endif