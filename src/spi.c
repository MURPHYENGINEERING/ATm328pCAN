#include "atm328p_spi.h"
#include "spi.h"
#include "memory.h"


FIFO_ENTRY_T g_spi_fifo[SPI_FIFO_LEN];
U8_T g_spi_fifo_head;
U8_T g_spi_fifo_tail;

SPI_FIFO_STATUS_T spi_tx_q_add(U8_T* src, U8_T len)
{
    SPI_FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if (g_spi_fifo_tail == g_spi_fifo_head - 1) {
        status = SPI_FIFO_FULL;
    } else {
        p_fifo_entry = &g_spi_fifo[g_spi_fifo_tail];
        memcpy_by_U8(p_fifo_entry->data, src, len);
        p_fifo_entry->len = len;

        ++g_spi_fifo_tail;
    }

    return status;
}


SPI_FIFO_STATUS_T spi_tx_q_remove(U8_T* dst, U8_T* len)
{
    SPI_FIFO_STATUS_T status;
    FIFO_ENTRY_T* p_fifo_entry;

    if (g_spi_fifo_head == g_spi_fifo_tail) {
        status =  SPI_FIFO_EMPTY;
    } else {
        p_fifo_entry = &g_spi_fifo[g_spi_fifo_head];
        memcpy_by_U8(dst, p_fifo_entry->data, p_fifo_entry->len);
        *len = p_fifo_entry->len;

        ++g_spi_fifo_head;
        status = SPI_FIFO_OK;
    }

    return status;
}


void spi_tx_task(void) {
    /* Dequeue a message from the software FIFO */
    U8_T buf[256];
    U8_T len;

    len = spi_tx_q_remove(buf, &len);
}
