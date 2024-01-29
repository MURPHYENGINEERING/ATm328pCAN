#include "types.h"
#include "atm328p/atm328p_dsc.h"
#include "atm328p/atm328p_spi.h"
#include "spi.h"
#include "interrupts.h"
#include "memory.h"
#include "fifo.h"
#include "dsc.h"


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
#define SPI_FIFO_TX_SIZE (SIZE_T) 4u
#define SPI_FIFO_RX_SIZE (SIZE_T) 4u

FIFO_ENTRY_T g_spi_tx_q_buf[SPI_FIFO_TX_SIZE];
FIFO_ENTRY_T g_spi_rx_q_buf[SPI_FIFO_RX_SIZE];

FIFO_T g_spi_tx_q;
FIFO_T g_spi_rx_q;
/******************************************************************************/

/******************************************************************************/
/* Master status flag */
VBOOL_T g_spi_ready;
/******************************************************************************/



ISR(SPI_STC_vect)
{
    g_spi_ready = TRUE;
}


void spi_init(void)
{
    fifo_q_init(&g_spi_tx_q, g_spi_tx_q_buf, SPI_FIFO_TX_SIZE);
    fifo_q_init(&g_spi_rx_q, g_spi_rx_q_buf, SPI_FIFO_RX_SIZE);

    /* Turn off Power Reduction for SPI */
    PRR.bits.PRSPI = PRSPI_DISABLE_POWER_REDUCTION;

    /* Enable "write finished" interrupts */
    SPCR.bits.SPIE = SPIE_ENABLE_SPI_INTERRUPT;
    /* MSB first */
    SPCR.bits.DORD = DORD_MSB_FIRST;
    /* Sampled on the Falling edge, */
    SPCR.bits.CPOL = CPOL_LEADING_EDGE_IS_RISING;
    SPCR.bits.CPHA = CPHA_SAMPLE_ON_LEADING_EDGE;
    /* at clk/128. */
    SPCR.byte |= SPCR_PRESCALE_OVER_128;

    /* Detect strapping for SPI mode */
    DDR_SPI.bits.MODE = DDR_INPUT;
    PORT_SPI.bits.MODE = PORT_PULLUP;
    if (HIGH == PIN_SPI.bits.MODE) {
        g_spi_mode = SPI_MODE_MASTER;
    }

    /* Set up SPI based on strapping */
    if (SPI_MODE_MASTER == g_spi_mode) {
        SPCR.bits.MSTR = MSTR_MODE_MASTER;
        DDR_SPI.bits.MOSI = DDR_OUTPUT;
        DDR_SPI.bits.SS = DDR_OUTPUT;
        DDR_SPI.bits.SCK = DDR_OUTPUT;
        PORT_SPI.bits.SS = HIGH;
    } else {
        SPCR.bits.MSTR = MSTR_MODE_SLAVE;
        DDR_SPI.bits.MISO = DDR_OUTPUT;
    }

    /* Enable SPI */
    SPCR.bits.SPE = SPE_ENABLE_SPI;

    g_spi_ready = TRUE;
}


void task_exec_spi_tx(void) {
    /* Read a message from the software FIFO and write it to hardware */
    U8_T buf[FIFO_DATA_LEN];
    U32_T len;
    SIZE_T i;
    FIFO_STATUS_T status;

    status = fifo_q_remove(&g_spi_tx_q, buf, &len);
    if (FIFO_OK == status) {
        PORT_SPI.bits.SS = LOW;
        for (i = 0; i < len; ++i) {
            g_spi_ready = FALSE;
            SPDR.byte = (VU8_T) buf[i];
            /* Wait for SPIF to be unset */
            while (FALSE == g_spi_ready) {
                dsc_led_set(DSC_LED_CANBOARD_2, DSC_LED_ON);
            }
            dsc_led_set(DSC_LED_CANBOARD_2, DSC_LED_OFF);
        }
        PORT_SPI.bits.SS = HIGH;
    } else { 
        /* Buffer underflow, nothing to transmit */
    }
}


void task_exec_spi_rx(void) {
    /* Read a message from hardware and write it to the software FIFO */
    U8_T buf[FIFO_DATA_LEN];
    U32_T len;
    FIFO_STATUS_T status;

    memcpy_by_U8(buf, (U8_T*) "Hello, world!", (U32_T) 13);
    len = 13;

    status = fifo_q_add(&g_spi_tx_q, buf, len);
    if (FIFO_OK == status) {

    } else { 
        /* Buffer overflow, packet is lost */
    }
}
