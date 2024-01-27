#ifndef __SPI_H__
#define __SPI_H__

#include "types.h"


typedef struct {
    U8_T data[256];
    U8_T len;
} FIFO_ENTRY_T;


typedef enum {
    SPI_FIFO_EMPTY,
    SPI_FIFO_FULL,
    SPI_FIFO_OK
} SPI_FIFO_STATUS_T;


#define SPI_FIFO_LEN 10
extern FIFO_ENTRY_T g_spi_fifo[SPI_FIFO_LEN];
extern U8_T g_spi_fifo_head;
extern U8_T g_spi_fifo_tail;


SPI_FIFO_STATUS_T spi_tx_q_add(U8_T* src, U8_T len);

SPI_FIFO_STATUS_T spi_tx_q_remove(U8_T* dst, U8_T* len);

void spi_tx_task(void);


#endif