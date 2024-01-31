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
TXBnCTRL_T TXB0CTRL;

#define TXB1CTRL_ADDR 0x40
TXBnCTRL_T TXB1CTRL;

#define TXB2CTRL_ADDR 0x50
TXBnCTRL_T TXB2CTRL;


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
TXRTSCTRL_T TXRTSCTRL;

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
TXBnSIDH_T TXB0SIDH;
TXBnSIDL_T TXB0SIDL;

#define TXB1SIDH_ADDR 0x41
#define TXB1SIDL_ADDR 0x42
TXBnSIDH_T TXB1SIDH;
TXBnSIDL_T TXB1SIDL;

#define TXB2SIDH_ADDR 0x51
#define TXB2SIDL_ADDR 0x52
TXBnSIDH_T TXB2SIDH;
TXBnSIDL_T TXB2SIDL;


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
TXBnDLC_T TXB0DLC;

#define TXB1DLC_ADDR 0x45
TXBnDLC_T TXB1DLC;

#define TXB2DLC_ADDR 0x55
TXBnDLC_T TXB2DLC;


/* Can Control Register */
typedef union {
    struct {
        /* Clock Out Prescaler bits */
        VBOOL_T CLKPRE0 : 1;
        VBOOL_T CLKPRE1 : 1;
        /* Clock Out Enable bit */
        VBOOL_T CLKEN   : 1;
        /* One-Shot Mode bit */
        VBOOL_T OSM     : 1;
        /* Abort All Pending Transmissions bit */
        VBOOL_T ABAT    : 1;
        /* Request Operation Mode bits */
        VBOOL_T REQOP0  : 1;
        VBOOL_T REQOP1  : 1;
        VBOOL_T REQOP2  : 1;
    } bits;
    VU8_T byte;
} CANCTRL_T;

#define CANCTRL_ADDR 0x0F
CANCTRL_T CANCTRL;

#define CANCTRL_MODE_NORMAL     0b00000000
#define CANCTRL_MODE_LOOPBACK   0b00000010
#define CANCTRL_MODE_CONFIG     0b00000100

/* Transmit Buffer n Data Byte m registers */
#define TXB0D_BASE_ADDR 0x36
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
/* Read a register */
#define CAN_MSG_READ        0b00000011
/* Write a register */
#define CAN_MSG_WRITE       0b00000010
#define CAN_MSG_READ_STATUS 0b10100000
#define CAN_MSG_RX_STATUS   0b10110000
#define CAN_MSG_BIT_MODIFY  0b00000101

/* Request To Send */
#define CAN_MSG_RTS         0b10000000
#define CAN_MSG_RTS_B0      0b00000001

/* Start writing directly into a data buffer */
#define CAN_MSG_LOAD_TX                     0b01000000
#define CAN_MSG_LOAD_TX_START_AT_TXB0SIDH   0b00000000
#define CAN_MSG_LOAD_TX_START_AT_TXB0D0     0b00000001
/******************************************************************************/


void can_init_hardware(void)
{
    U8_T hold_reset;

    spi_activate();
        spi_tx_rx(CAN_MSG_RESET);
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
        spi_tx_rx(CAN_MSG_WRITE);
        spi_tx_rx(CANCTRL_ADDR);
        spi_tx_rx(CANCTRL.byte);
    spi_deactivate();
}


void can_tx(U16_T identifier, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    /* Transfer the buffer into the data registers */
    spi_activate();
        /* Select Buffer 0, Start at TXB0D0 */
        spi_tx_rx((U8_T)( CAN_MSG_LOAD_TX | CAN_MSG_LOAD_TX_START_AT_TXB0D0 ));
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
        spi_tx_rx(CAN_MSG_WRITE);
        spi_tx_rx(TXB0DLC_ADDR);
        /* It's ok if len > 8, MCP2515 will ignore anything past 8 bytes. */
        spi_tx_rx((U8_T)( len & DLC_MASK ));
    spi_deactivate();

    /* Trigger the transmission */
    spi_activate();
        /* Request To Send the 0th buffer */
        spi_tx_rx((U8_T)( CAN_MSG_RTS | CAN_MSG_RTS_B0 ));
    spi_deactivate();
}

