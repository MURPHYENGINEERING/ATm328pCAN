#include "types.h"
#include "atm328p_spi.h"
#include "atm328p_mcu.h"
#include "atm328p_dsc.h"

#include "spi.h"


/*******************************************************************************
 *
 ******************************************************************************/
void spi_init(void)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    VU8_T dummy_read;
    #pragma GCC diagnostic pop

    /* Turn off Power Reduction for SPI */
    PRR.bits.PRSPI = PRSPI_DISABLE_POWER_REDUCTION;

    /* MSB first */
    SPCR.bits.DORD = DORD_MSB_FIRST;
    /* Sampled on the Falling edge, */
    SPCR.bits.CPOL = CPOL_LEADING_EDGE_IS_RISING;
    SPCR.bits.CPHA = CPHA_SAMPLE_ON_LEADING_EDGE;
    /* at clk/128. */
    SPCR.byte |= SPCR_PRESCALE_OVER_128;

    /* Set up pins for MASTER mode */
    SPCR.bits.MSTR = MSTR_MODE_MASTER;
    DDR_SPI.bits.MOSI = DDR_OUTPUT;
    DDR_SPI.bits.SS = DDR_OUTPUT;
    DDR_SPI.bits.SCK = DDR_OUTPUT;
    /* Default to no slave selected */
    PORT_SPI.bits.SS = HIGH;

    /* Enable "write finished" interrupts */
    SPCR.bits.SPIE = SPIE_ENABLE_SPI_INTERRUPT;

    /* Enable SPI */
    SPCR.bits.SPE = SPE_ENABLE_SPI;

    /* Clear the SPI buffers */
    dummy_read = SPSR.byte;
    dummy_read = SPDR.byte;

    g_spi_ready = TRUE;
}


/*******************************************************************************
 *
 ******************************************************************************/
void spi_activate(void) {
    PORT_SPI.bits.SS = LOW;
}


/*******************************************************************************
 *
 ******************************************************************************/
void spi_deactivate(void) {
    PORT_SPI.bits.SS = HIGH;
}


/*******************************************************************************
 *
 ******************************************************************************/
U8_T spi_tx_rx(U8_T tx_data) {
    U8_T rx_data;

    g_spi_ready = FALSE;
    SPDR.byte = tx_data;
    /* Wait for "data written" interrupt */
    while (FALSE == g_spi_ready) {
    }
    /* Read the shifted-in byte */
    rx_data = SPDR.byte;

    return rx_data;
}