#include "can.h"
#include "fifo.h"
#include "spi.h"
#include "dsc.h"

/******************************************************************************/
/* CAN Queues */
#define CAN_FIFO_TX_SIZE (SIZE_T) 16u
#define CAN_FIFO_RX_SIZE (SIZE_T) 16u

FIFO_ENTRY_T g_can_tx_q_buf[CAN_FIFO_TX_SIZE];
FIFO_ENTRY_T g_can_rx_q_buf[CAN_FIFO_RX_SIZE];

FIFO_T g_can_tx_q;
FIFO_T g_can_rx_q;
/******************************************************************************/

/******************************************************************************/
/* CAN Modes */
#define CAN_MODE_NORMAL     0b00000000
/******************************************************************************/

/******************************************************************************/
/* CAN Registers */
#define CAN_REG_TXB0CTRL 0x30
#define CAN_REG_TX0RTS 0x0D
/******************************************************************************/

/******************************************************************************/
/* CAN Messages */
#define CAN_MSG_RESET       0b11000000
#define CAN_MSG_READ        0b00000011
#define CAN_MSG_WRITE       0b00000010
#define CAN_MSG_READ_STATUS 0b10100000
#define CAN_MSG_RX_STATUS   0b10110000
#define CAN_MSG_BIT_MODIFY  0b00000101
#define CAN_MSG_RTS         0b10000000
/******************************************************************************/

/******************************************************************************/
/* Statics */
static void can_tx(U8_T* buf, SIZE_T len);
/******************************************************************************/


void can_init(void)
{
    U8_T hold_reset;

    fifo_q_init(&g_can_tx_q, g_can_tx_q_buf, CAN_FIFO_TX_SIZE);
    fifo_q_init(&g_can_rx_q, g_can_rx_q_buf, CAN_FIFO_RX_SIZE);

    spi_activate();
    spi_tx_rx(CAN_MSG_RESET);
    for (hold_reset = 0; U8_T_MAX > hold_reset; ++hold_reset) {
        /* Hold reset for 255 cycles per MCP2515 eval sample */
        /* Even though it appears to hold for 48-us without this,
         * which exceeds the 2-us requirement. */
    }
    spi_deactivate();

    spi_activate();
    spi_tx_rx(CAN_MSG_BIT_MODIFY);
    spi_tx_rx(CAN_MODE_NORMAL);
    spi_deactivate();
}


FIFO_STATUS_T can_tx_q_add(U8_T* buf, SIZE_T len)
{
    FIFO_STATUS_T status;

    status = fifo_q_add(&g_can_tx_q, buf, len);

    return status;
}


void task_can_tx(void)
{
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T n_pending_msgs;
    SIZE_T len;
    SIZE_T i;

    n_pending_msgs = fifo_q_len(&g_can_tx_q);

    for (i = 0; i < n_pending_msgs; ++i) {
        status = fifo_q_remove(&g_can_tx_q, buf, &len);

        if (FIFO_OK == status) {
            can_tx(buf, len);
        } else {
            /* Shouldn't underflow, report software fault */
        }
    }

    dsc_led_set(DSC_LED_CANBOARD_2, ON);
}


static void can_tx(U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    /* TODO: This doesn't work! */

    spi_activate();
    spi_tx_rx(CAN_MSG_WRITE);
    spi_tx_rx((U8_T) 0);
    for (i = 0; i < len; ++i) {
        spi_tx_rx(buf[i]);
    }
    spi_deactivate();

    spi_activate();
    spi_tx_rx(CAN_REG_TXB0CTRL);
    spi_tx_rx((U8_T) 0x0);
    spi_deactivate();

    spi_activate();
    spi_tx_rx(CAN_MSG_RTS | 0b0000000);
    spi_deactivate();

    spi_activate();
    spi_tx_rx(CAN_REG_TX0RTS);
    spi_tx_rx((U8_T) 0b00000001);
}


SIZE_T can_rx_q_len(void)
{
    return fifo_q_len(&g_can_rx_q);
}


void task_can_rx(void)
{
    spi_activate();
    spi_tx_rx(CAN_MSG_READ);
    spi_deactivate();

    /* TODO: read a message */

    dsc_led_set(DSC_LED_CANBOARD_2, OFF);
}