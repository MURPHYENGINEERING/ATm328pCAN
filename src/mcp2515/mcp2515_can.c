#include "types.h"
#include "mcp2515_can.h"
#include "can.h"
#include "fifo.h"
#include "spi.h"
#include "dsc.h"


/*******************************************************************************
 * Initialize the MCP2515 CAN controller by writing to its registers via SPI.
 ******************************************************************************/ 
void can_init_hardware(void)
{
    U8_T hold_reset;

    spi_begin();
        spi_tx_rx(CAN_CMD_RESET);
        for (hold_reset = 0; U8_T_MAX > hold_reset; ++hold_reset) {
            /* Hold reset for 255 cycles per MCP2515 eval sample */
            /* Works out to 4.6-us, which beats the 2-us requirement */
        }
    spi_end();

    /* Disable One-Shot Mode, CLKOUT pin, and don't Abort */
    CANCTRL.byte = (U8_T) 0;
    /* Exit configuration mode */
    CANCTRL.byte |= CANCTRL_MODE_NORMAL;

    spi_begin();
        spi_tx_rx(CAN_CMD_WRITE);
        spi_tx_rx(CANCTRL_ADDR);
        spi_tx_rx(CANCTRL.byte);
    spi_end();
}


/*******************************************************************************
 * Write the given message to the CAN hardware via SPI.
 *
 * This function manipulates the MCP2515 via SPI to write the given identifier
 * and data to the first available hardware tx buffer.
 *
 * \param[in] identifier    11-bit CAN message identifier.
 * \param[in] buf           Pointer to data buffer to populate CAN message with.
 * \param[in] len           Length of data in the given data buffer.
 ******************************************************************************/     
void can_tx(CAN_IDENT_T identifier, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    /* Transfer the identifier into the SID registers */
    spi_begin();
        /* Start writing at TXB0SIDH */
        spi_tx_rx((U8_T)( CAN_CMD_LOAD_TX | CAN_CMD_LOAD_TX_START_AT_TXB0SIDH ));
        spi_tx_rx(SID_GET_HIGH_BITS(identifier));
        spi_tx_rx(SID_GET_LOW_BITS(identifier));
    spi_end();

    /* Transfer the buffer into the data registers */
    spi_begin();
        /* Start writing at TXB0D0 */
        spi_tx_rx((U8_T)( CAN_CMD_LOAD_TX | CAN_CMD_LOAD_TX_START_AT_TXB0D0 ));
        /* It is possible to write to sequential registers by continuing to 
         * clock in data bytes as long as CS is held low.
         * - MCP2515 Datasheet page 65 */
        for (i = 0; (8 > i) && (i < len); ++i) {
            spi_tx_rx(buf[i]);
        }
        /* Zero the unused portion of the frame. */
        for (; 8 > i; ++i) {
            spi_tx_rx((U8_T) 0);
        }
    spi_end();
    
    /* Write the data length */
    /* The masking has the side effect of setting RTR to Data Frame. */
    spi_begin();
        spi_tx_rx(CAN_CMD_WRITE);
        spi_tx_rx(TXB0DLC_ADDR);
        /* It's ok if len > 8, MCP2515 will ignore anything past 8 bytes. */
        spi_tx_rx((U8_T)( len & DLC_MASK ));
    spi_end();

    /* Enable Message Transmit Request */
    TXB0CTRL.byte = (U8_T) 0;
    TXB0CTRL.bits.TXP0 = TXP_PRIORITY_HIGH;
    TXB0CTRL.bits.TXP1 = TXP_PRIORITY_HIGH;
    TXB0CTRL.bits.TXREQ = TXREQ_TX_START;
    spi_begin();
        spi_tx_rx(CAN_CMD_WRITE);
        spi_tx_rx(TXB0CTRL_ADDR);
        spi_tx_rx(TXB0CTRL.byte);
    spi_end();

    /* Trigger the transmission */
    spi_begin();
        /* Request To Send the 0th buffer */
        spi_tx_rx((U8_T)( CAN_CMD_RTS | CAN_CMD_RTS_B0 ));
    spi_end();
}


/*******************************************************************************
 * Retrieve the appropriate SPI configuration to interact with the MCP2515.
 * \return  The configuration that configures the SPI device to interact with the
 *          MCP2515.
 ******************************************************************************/     
SPI_CONFIG_T can_get_spi_config(void)
{
    SPI_CONFIG_T config;
    config.enable       = ENABLED;
    config.mode         = SPI_MODE_MASTER;
    /* MCP2515 Datasheet page 71 */
    config.endian       = SPI_ENDIAN_MSB_FIRST;
    /* MCP2515 Datasheet page 65: mode 0,0 */
    config.phase        = SPI_PHASE_SAMPLE_ON_LEADING;
    config.polarity     = SPI_POLARITY_LEADING_IS_RISING;
    config.prescale     = SPI_PRESCALE_OVER_256;
    /* The SPI driver is not interrupt driven. */
    config.interrupts   = DISABLED;

    return config;
}