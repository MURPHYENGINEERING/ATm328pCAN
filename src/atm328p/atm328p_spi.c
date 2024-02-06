#include "types.h"
#include "atm328p_spi.h"
#include "atm328p_mcu.h"
#include "atm328p_dsc.h"

#include "spi.h"
#include "fai.h"
#include "memory.h"


/*******************************************************************************
 * Initialize the SPI device hardware by setting registers on the microcontroller.
 * Additionally, clear the SPI read and write buffers, and set the state of the
 * SPI software device to "ready."
 ******************************************************************************/
void spi_init(SPI_CFG_T cfg)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    VU8_T dummy_read;
    #pragma GCC diagnostic pop

    SPCR.byte = (U8_T) 0u;

    /* Turn off Power Reduction for SPI */
    if (ENABLED == cfg.enable) {
        PRR.bits.PRSPI = PRSPI_ENABLE_SPI;
    } else {
        PRR.bits.PRSPI = PRSPI_DISABLE_SPI;
    }

    switch (cfg.endian) {
    case SPI_ENDIAN_MSB_FIRST:
        SPCR.bits.DORD = DORD_MSB_FIRST;
        break;
    case SPI_ENDIAN_LSB_FIRST:
        SPCR.bits.DORD = DORD_LSB_FIRST;
        break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        break;
    }

    switch (cfg.polarity) {
    case SPI_POLARITY_LEADING_IS_RISING:
        SPCR.bits.CPOL = CPOL_LEADING_EDGE_IS_RISING;
        break;
    case SPI_POLARITY_LEADING_IS_FALLING:
        SPCR.bits.CPOL = CPOL_LEADING_EDGE_IS_FALLING;
        break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        break;
    }

    switch (cfg.phase) {
    case SPI_PHASE_SAMPLE_ON_LEADING:
        SPCR.bits.CPHA = CPHA_SAMPLE_ON_LEADING_EDGE;
        break;
    case SPI_PHASE_SETUP_ON_LEADING:
        SPCR.bits.CPHA = CPHA_SETUP_ON_LEADING_EDGE;
        break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        break;
    }

    switch (cfg.prescale) {
    case SPI_PRESCALE_OVER_256:
        SPCR.byte |= SPCR_PRESCALE_OVER_128;
        break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        break;
    }

    switch (cfg.mode) {
    case SPI_MODE_MASTER:
        SPCR.bits.MSTR = MSTR_MODE_MASTER;
        DDR_SPI.bits.MOSI = DDR_OUTPUT;
        DDR_SPI.bits.SS = DDR_OUTPUT;
        /* Default to no slave selected */
        PORT_SPI.bits.SS = HIGH;
        DDR_SPI.bits.SCK = DDR_OUTPUT;
        break;
    case SPI_MODE_SLAVE:
        SPCR.bits.MSTR = MSTR_MODE_SLAVE;
        break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        break;
    }


    if (ENABLED == cfg.interrupts) {
        /* Enable "write finished" interrupts */
        SPCR.bits.SPIE = SPIE_ENABLE_SPI_INTERRUPT;
    } else {
        SPCR.bits.SPIE = SPIE_DISABLE_SPI_INTERRUPT;
    }

    if (ENABLED == cfg.enable) {
        /* Enable SPI */
        SPCR.bits.SPE = SPE_ENABLE_SPI;
        /* Clear the SPI buffers */
        dummy_read = SPSR.byte;
        dummy_read = SPDR.byte;
    } else {
        SPCR.bits.SPE = SPE_DISABLE_SPI;
    }

}


/*******************************************************************************
 * Pull the SPI device chip select line DOWN to activate the device.
 ******************************************************************************/
void spi_begin(void) {
    PORT_SPI.bits.SS = LOW;
}


/*******************************************************************************
 * Pull the SPI device chip select line UP to deactivate the device.
 ******************************************************************************/
void spi_end(void) {
    PORT_SPI.bits.SS = HIGH;
}


/*******************************************************************************
 * Transmit the given byte to the SPI device hardware and receive the shifted-in
 * response byte.
 * \param[in] tx_data The byte to be shifted out to the SPI device shift register.
 * \return The byte shifted in to the SPI device shift register.
 ******************************************************************************/
U8_T spi_tx_rx(U8_T tx_data) {
    U8_T rx_data;

    SPDR.byte = tx_data;
    /* Wait for the previous transfer to complete. */
    /* "The SPIF bit is cleared by first reading the SPI status register with
     * SPIF set, then accessing the SPI data register (SPDR)."
     * - ATmega328P Datasheet page 141 */
    while (FALSE == SPSR.bits.SPIF) {
    }
    /* Read the shifted-in byte, clearing SPIF */
    rx_data = SPDR.byte;

    return rx_data;
}