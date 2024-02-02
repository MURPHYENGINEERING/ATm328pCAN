#ifndef __CAN_H__
#define __CAN_H__

#include "types.h"
#include "fifo.h"


/** Maximum number of elements in the CAN TX FIFO queue. */
#define CAN_FIFO_TX_SIZE (SIZE_T) 4u
/** Maximum number of elements in the CAN RX FIFO queue. */
#define CAN_FIFO_RX_SIZE (SIZE_T) 4u
/** Maximum data size for any CAN message in a FIFO queue. */
#define CAN_FIFO_DATA_LEN (SIZE_T) 8u

/** Represents an 11-bit CAN message identifier. */
typedef U16_T CAN_IDENT_T;

/** Represents a particular element in the CAN FIFO queue. */
typedef struct {
    /** CAN message identifier associated with this queue element */
    CAN_IDENT_T identifier;
    /** Data associated with this queue element */
    U8_T data[CAN_FIFO_DATA_LEN];
    /** Length of data in the `data` array */
    SIZE_T len;
} CAN_FIFO_ENTRY_T;

/** A FIFO queue of CAN messages. */
typedef struct
{
    /** Underlying buffer */
    CAN_FIFO_ENTRY_T* buf;
    /** Underlying buffer size */
    SIZE_T size;
    /** Read pointer */
    U8_T head;
    /** Write pointer */
    U8_T tail;
    /** Number of entries */
    SIZE_T n;
} CAN_FIFO_T;


/** CAN transmit queue underlying buffer */
extern CAN_FIFO_ENTRY_T g_can_tx_q_buf[CAN_FIFO_TX_SIZE];
/** CAN receive queue underlying buffer */
extern CAN_FIFO_ENTRY_T g_can_rx_q_buf[CAN_FIFO_RX_SIZE];

/** CAN transmit FIFO queue */
extern CAN_FIFO_T g_can_tx_q;
/** CAN receive FIFO queue */
extern CAN_FIFO_T g_can_rx_q;


void can_init(void);
void can_init_hardware(void);

void can_tx(CAN_IDENT_T identifier, U8_T* buf, SIZE_T len);

void can_fifo_q_init(CAN_FIFO_T* q, CAN_FIFO_ENTRY_T* buf, SIZE_T size);

FIFO_STATUS_T can_fifo_q_add(
    CAN_FIFO_T* q, 
    CAN_IDENT_T identifier, 
    U8_T* src, 
    SIZE_T len
);

FIFO_STATUS_T can_fifo_q_remove(
    CAN_FIFO_T* q, 
    CAN_IDENT_T* identifier, 
    U8_T* dst, 
    SIZE_T* len
);

SIZE_T can_fifo_q_len(CAN_FIFO_T* q);

FIFO_STATUS_T can_tx_q_add(CAN_IDENT_T identifier, U8_T* buf, SIZE_T len);
FIFO_STATUS_T can_rx_q_remove(CAN_IDENT_T* identifier, U8_T* buf, SIZE_T* len);
SIZE_T can_rx_q_len(void);

void task_can_tx(void);
void task_can_rx(void);



#endif