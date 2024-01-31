#include "types.h"
#include "mcp2515_can.h"
#include "can.h"
#include "fifo.h"
#include "spi.h"
#include "dsc.h"


/******************************************************************************/
/* CAN Modes */
#define CAN_MODE_NORMAL     0b00000000
/******************************************************************************/

/******************************************************************************/
/* CAN Registers */
#define TX0RTS_ADDR 0x0D

/* Transmit Buffer n Control Register */
typedef union
{
    struct {
        /* Transmit Buffer Priority Bits (R/W) */
        VBOOL_T TXP0        : 1;
        VBOOL_T TXP1        : 1;
        VBOOL_T Reserved2   : 1
        /* Message Transmit Request Bit (R/W) */
        VBOOL_T TXREQ       : 1;
        /* Transmission Error Detected Bit (R) */
        VBOOL_T TXERR       : 1;
        /* Message Lost Arbitration Bit (R) */
        VBOOL_T MLOA        : 1;
        /* Message Aborted Flag Bit (R) */
        VBOOL_T ABTF        : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TXBnCTRL_T;

#define ABTF_MESSAGE_ABORTED 1u
#define ABTF_MESSAGE_TRANSMISSION_SUCCESSFUL 0
#define MLOA_MESSAGE_LOST_ARBITRATION 1u
#define TXERR_BUS_ERROR_DETECTED 1u
#define TXREQ_BUFFER_PENDING_TRANSMISSION 1u
#define TXREQ_BUFFER_NOT_PENDING_TRANSMISSION 0
#define TXP0_PRIORITY_HIGH 1u
#define TXP0_PRIORITY_LOW 0

#define TXB0CTRL_ADDR 0x30
volatile TXBnCTRL_T TXB0CTRL;

#define TXB1CTRL_ADDR 0x40
volatile TXBnCTRL_T TXB1CTRL;

#define TXB2CTRL_ADDR 0x50
volatile TXBnCTRL_T TXB2CTRL;


/* ~TXnRTS Pin Control and Status Register */
typedef union
{
    struct {
        /* ~TX0RTS Pin Mode (R/W) */
        VBOOL_T B0RTSM      : 1;
        /* ~TX1RTS Pin Mode (R/W) */
        VBOOL_T B1RTSM      : 1;
        /* ~TX2RTS Pin Mode (R/W) */
        VBOOL_T B2RTSM      : 1;
        /* ~TX0RTS Pin State (R) */
        VBOOL_T B0RTS       : 1;
        /* ~TX1RTS Pin State (R) */
        VBOOL_T B1RTS       : 1;
        /* ~TX2RTS Pin State (R) */
        VBOOL_T B2RTS       : 1;
        VBOOL_T Reserved6   : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TXRTSCTRL_T;

#define BnRTSM_PIN_REQUESTS_TRANSMISSION 1u
#define BnRTS_REQUEST_TO_SEND 0

#define TXRTSCTRL_ADDR 0x0D
volatile TXRTSCTRL_T TXRTSCTRL;

/* Transmit Buffer n Standard Identifier Register */
typedef union 
{
    struct {
        /* Standard Identifier bits */
        VBOOL_T SID3  : 1;
        VBOOL_T SID4  : 1;
        VBOOL_T SID5  : 1;
        VBOOL_T SID6  : 1;
        VBOOL_T SID7  : 1;
        VBOOL_T SID8  : 1;
        VBOOL_T SID9  : 1;
        VBOOL_T SID10 : 1;
    } bits;
    VU8_T byte;
} TXBnSIDH_T;

typedef union 
{
    struct {
        /* Extended Identifier bits */
        VBOOL_T EID16       : 1;
        VBOOL_T EID17       : 1;
        VBOOL_T Reserved2   : 1;
        /* Extended Identifier Enable bit */
        VBOOL_T EXIDE       : 1;
        VBOOL_T Reserved4   : 1;
        /* Standard Identifier bits */
        VBOOL_T SID0        : 1;
        VBOOL_T SID1        : 1;
        VBOOL_T SID2        : 1;
    } bits;
    VU8_T byte;
} TXBnSIDL_T;

#define EXIDE_STANDARD_IDENTIFIER 0

/* Masks off only the Standard Identifier bits */
#define SIDL_MASK (U8_T) 0b11100000
/* Number of bits to right-shift an identifier to get the high bits */
#define SIDH_SHIFT 3
/* Masks off only the high bits, after shifting */
#define SIDH_MASK (U8_T) 0xFF
#define SID_GET_HIGH_BITS(i) (U8_T)((i >> SIDH_SHIFT) & SIDH_MASK)
#define SID_GET_LOW_BITS(i)  (U8_T)(i & SIDL_MASK)

#define TXB0SIDH_ADDR 0x31
#define TXB0SIDL_ADDR 0x32
volatile TXBnSIDH_T TXB0SIDH;
volatile TXBnSIDL_T TXB0SIDL;

#define TXB1SIDH_ADDR 0x41
#define TXB1SIDL_ADDR 0x42
volatile TXBnSIDH_T TXB1SIDH;
volatile TXBnSIDL_T TXB1SIDL;

#define TXB2SIDH_ADDR 0x51
#define TXB2SIDL_ADDR 0x52
volatile TXBnSIDH_T TXB2SIDH;
volatile TXBnSIDL_T TXB2SIDL;


/* Transmit Buffer n Data Length Code Register */
typedef union
{
    struct {
        /* Data Length Code bits */
        VBOOL_T DLC0        : 1;
        VBOOL_T DLC1        : 1;
        VBOOL_T DLC2        : 1;
        VBOOL_T DLC3        : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        /* Remote Transmission Request bit */
        VBOOL_T RTR         : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TXBnDLC_T;

#define RTR_REMOTE_TRANSMISSION 1u
#define RTR_DATA_FRAME 0

#define DLC_MASK 0b00001111

#define TXB0DLC_ADDR 0x35
volatile TXBnDLC_T TXB0DLC;

#define TXB1DLC_ADDR 0x45
volatile TXBnDLC_T TXB1DLC;

#define TXB2DLC_ADDR 0x55
volatile TXBnDLC_T TXB2DLC;

/* Transmit Buffer n Data Byte m registers */
#define TXB0D0_ADDR 0x36
#define TXB0D1_ADDR 0x37
#define TXB0D2_ADDR 0x38
#define TXB0D3_ADDR 0x39
#define TXB0D4_ADDR 0x3A
#define TXB0D5_ADDR 0x3B
#define TXB0D6_ADDR 0x3C
#define TXB0D7_ADDR 0x3D
volatile REGISTER_T TXB0Dm;

#define TXB1D0_ADDR 0x46
#define TXB1D1_ADDR 0x47
#define TXB1D2_ADDR 0x48
#define TXB1D3_ADDR 0x49
#define TXB1D4_ADDR 0x4A
#define TXB1D5_ADDR 0x4B
#define TXB1D6_ADDR 0x4C
#define TXB1D7_ADDR 0x4D
volatile REGISTER_T TXB1Dm;

#define TXB2D0_ADDR 0x56
#define TXB2D1_ADDR 0x57
#define TXB2D2_ADDR 0x58
#define TXB2D3_ADDR 0x59
#define TXB2D4_ADDR 0x5A
#define TXB2D5_ADDR 0x5B
#define TXB2D6_ADDR 0x5C
#define TXB2D7_ADDR 0x5D
volatile REGISTER_T TXB2Dm;

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


void can_init_hardware(void)
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


FIFO_STATUS_T can_tx_q_add(U16_T identifier, U8_T* buf, SIZE_T len)
{
    FIFO_STATUS_T status;

    status = can_fifo_q_add(&g_can_tx_q, identifier, buf, len);

    return status;
}


FIFO_STATUS_T can_rx_q_remove(U16_T* identifier, U8_T* buf, SIZE_T* len)
{
    FIFO_STATUS_T status;

    status = can_fifo_q_remove(&g_can_rx_q, identifier, buf, len);

    return status;
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