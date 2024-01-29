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
/* CAN Messages */
#define CAN_MSG_RESET       0b11000000
#define CAN_MSG_READ        0b00000011
#define CAN_MSG_WRITE       0b00000010
#define CAN_MSG_READ_STATUS 0b10100000
#define CAN_MSG_RX_STATUS   0b10110000
#define CAN_MSG_BIT_MODIFY  0b00000101
/******************************************************************************/

void can_init(void)
{
    fifo_q_init(&g_can_tx_q, g_can_tx_q_buf, CAN_FIFO_TX_SIZE);
    fifo_q_init(&g_can_rx_q, g_can_rx_q_buf, CAN_FIFO_RX_SIZE);

    spi_tx_rx(CAN_MSG_RESET);
}


FIFO_STATUS_T can_q_add(U8_T* buf, SIZE_T len)
{
    FIFO_STATUS_T status;

    status = fifo_q_add(&g_can_tx_q, buf, len);

    return status;
}


void task_can_tx(void)
{
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T len;
    SIZE_T i;

    status = fifo_q_remove(&g_can_tx_q, buf, &len);
    if (FIFO_OK == status) {
        spi_activate();
        spi_tx_rx(CAN_MSG_WRITE);
        for (i = 0; i < len; ++i) {
            spi_tx_rx(buf[i]);
        }
        spi_deactivate();
        
        dsc_led_set(DSC_LED_CANBOARD_2, DSC_LED_ON);

    } else {
        /* Buffer underrun, do nothing */
    }
}


void task_can_rx(void)
{
    spi_activate();

    spi_deactivate();

    dsc_led_set(DSC_LED_CANBOARD_2, DSC_LED_OFF);
}