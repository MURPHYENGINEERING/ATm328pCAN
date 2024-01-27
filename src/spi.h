#ifndef __SPI_H__
#define __SPI_H__

#include "types.h"

#define SPI_FIFO_LEN 4
extern FIFO_ENTRY_T g_spi_fifo[SPI_FIFO_LEN];
extern U8_T g_spi_fifo_head;
extern U8_T g_spi_fifo_tail;


FIFO_STATUS_T spi_tx_q_add(U8_T* src, U32_T len);

FIFO_STATUS_T spi_tx_q_remove(U8_T* dst, U32_T* len);

void spi_tx_task(void);


#endif