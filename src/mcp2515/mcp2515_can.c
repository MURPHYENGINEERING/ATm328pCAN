#include "types.h"
#include "mcp2515_can.h"
#include "can.h"
#include "fifo.h"
#include "spi.h"
#include "dsc.h"


/***************************************************************************//**
 *
 ******************************************************************************/ 
void can_init_hardware(void)
{
    U8_T hold_reset;

    spi_activate();
        spi_tx_rx(CAN_CMD_RESET);
        for (hold_reset = 0; U8_T_MAX > hold_reset; ++hold_reset) {
            /* Hold reset for 255 cycles per MCP2515 eval sample */
            /* Even though it appears to hold for 48-us without this,
            * which exceeds the 2-us requirement. */
        }
    spi_deactivate();

    /* Disable One-Shot Mode, CLKOUT pin, and don't Abort */
    CANCTRL.byte = (U8_T) 0;
    /* Exit configuration mode */
    CANCTRL.byte |= CANCTRL_MODE_NORMAL;

    spi_activate();
        spi_tx_rx(CAN_CMD_WRITE);
        spi_tx_rx(CANCTRL_ADDR);
        spi_tx_rx(CANCTRL.byte);
    spi_deactivate();
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
    spi_activate();
        /* Start writing at TXB0SIDH */
        spi_tx_rx((U8_T)( CAN_CMD_LOAD_TX | CAN_CMD_LOAD_TX_START_AT_TXB0SIDH ));
        spi_tx_rx(SID_GET_HIGH_BITS(identifier));
        spi_tx_rx(SID_GET_LOW_BITS(identifier));
    spi_deactivate();

    /* Transfer the buffer into the data registers */
    spi_activate();
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
    spi_deactivate();
    
    /* Write the data length */
    /* The masking has the side effect of setting RTR to Data Frame. */
    spi_activate();
        spi_tx_rx(CAN_CMD_WRITE);
        spi_tx_rx(TXB0DLC_ADDR);
        /* It's ok if len > 8, MCP2515 will ignore anything past 8 bytes. */
        spi_tx_rx((U8_T)( len & DLC_MASK ));
    spi_deactivate();

    /* Trigger the transmission */
    spi_activate();
        /* Request To Send the 0th buffer */
        spi_tx_rx((U8_T)( CAN_CMD_RTS | CAN_CMD_RTS_B0 ));
    spi_deactivate();
}

