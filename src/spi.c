#include "types.h"
#include "atm328p/atm328p_dsc.h"
#include "atm328p/atm328p_spi.h"
#include "spi.h"
#include "memory.h"
#include "fifo.h"

/******************************************************************************/
/* SPI Master/Slave Mode */
typedef enum {
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE
} SPI_MODE_T;

SPI_MODE_T g_spi_mode;
/******************************************************************************/

/******************************************************************************/
/* SPI Queues */
#define SPI_FIFO_TX_SIZE 4
#define SPI_FIFO_RX_SIZE 4

FIFO_ENTRY_T g_spi_tx_q_buf[SPI_FIFO_TX_SIZE];
FIFO_ENTRY_T g_spi_rx_q_buf[SPI_FIFO_RX_SIZE];

FIFO_T g_spi_tx_q;
FIFO_T g_spi_rx_q;
/******************************************************************************/



void spi_init(void)
{
    fifo_q_init(&g_spi_tx_q, g_spi_tx_q_buf, SPI_FIFO_TX_SIZE);
    fifo_q_init(&g_spi_rx_q, g_spi_rx_q_buf, SPI_FIFO_RX_SIZE);

    /* Turn off Power Reduction for SPI */
    PRR.bits.PRSPI = PRSPI_DISABLE_POWER_REDUCTION;

    /* Enable SPI in master mode with interrupts enabled, */
    SPCR.bits.SPE = SPE_ENABLE_SPI;
    SPCR.bits.SPIE = SPIE_ENABLE_SPI_INTERRUPT;
    /* Sampled on the Falling edge, */
    SPCR.bits.CPOL = CPOL_LEADING_IS_RISING;
    SPCR.bits.CPHA = CPHA_SAMPLE_ON_LEADING;
    /* at clk/128. */
    SPCR.byte |= SPCR_PRESCALE_OVER_128;

    /* Setup output and input pins */
    DDR_SPI.bits.SCK = DDR_OUTPUT;
    DDR_SPI.bits.SS = DDR_OUTPUT;

    /* Detect strapping for SPI mode */
    DDR_SPI.bits.MODE = DDR_INPUT;
    PORT_SPI.bits.MODE = PORT_PULLUP;
    if (LOW == PIN_SPI.bits.MODE) {
        g_spi_mode = SPI_MODE_MASTER;
    }

    /* Set up SPI based on strapping */
    if (SPI_MODE_MASTER == g_spi_mode) {
        SPCR.bits.MSTR = MSTR_MODE_MASTER;
        DDR_SPI.bits.MOSI = DDR_OUTPUT;
        DDR_SPI.bits.MISO = DDR_INPUT;
    } else {
        SPCR.bits.MSTR = MSTR_MODE_SLAVE;
        DDR_SPI.bits.MOSI = DDR_INPUT;
        DDR_SPI.bits.MISO = DDR_OUTPUT;
    }
}


void spi_tx_task(void) {
    /* Dequeue a message from the software FIFO */
    U8_T buf[256];
    U32_T len;
    FIFO_STATUS_T status;

    status = fifo_q_remove(&g_spi_tx_q, buf, &len);
    if (FIFO_OK == status) {

    } else { 
    }
}
