#include "atm328p_spi.h"
#include "spi.h"
#include "memory.h"


FIFO_ENTRY_T g_spi_q_buf[SPI_FIFO_LEN];


void spi_q_init(void)
{
    g_spi_q.buf = g_spi_q_buf;
    g_spi_q.len = SPI_FIFO_LEN;
    g_spi_q.head = (U8_T) 0;
    g_spi_q.tail = (U8_T) 0;
}


void spi_tx_task(void) {
    /* Dequeue a message from the software FIFO */
    U8_T buf[256];
    U32_T len;

    len = fifo_q_remove(&g_spi_q, buf, &len);
}
