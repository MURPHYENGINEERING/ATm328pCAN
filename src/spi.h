#ifndef __SPI_H__
#define __SPI_H__

#include "types.h"
#include "fifo.h"

#define SPI_FIFO_LEN 4
extern FIFO_ENTRY_T g_spi_q_buf[SPI_FIFO_LEN];
FIFO_T g_spi_q;


void spi_q_init(void);

void spi_tx_task(void);


#endif