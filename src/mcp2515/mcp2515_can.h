#ifndef __MCP2515_CAN_H__
#define __MCP2515_CAN_H__


/** Address of the TX0RTS (Request To Send) register. */
#define TX0RTS_ADDR 0x0D

/** Transmit Buffer n Control Register */
typedef union
{
    struct {
        /** Transmit Buffer Priority Bits (R/W) */
        VBOOL_T TXP0        : 1;
        VBOOL_T TXP1        : 1;
        VBOOL_T Reserved2   : 1;
        /** Message Transmit Request Bit (R/W) */
        VBOOL_T TXREQ       : 1;
        /** Transmission Error Detected Bit (R) */
        VBOOL_T TXERR       : 1;
        /** Message Lost Arbitration Bit (R) */
        VBOOL_T MLOA        : 1;
        /** Message Aborted Flag Bit (R) */
        VBOOL_T ABTF        : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TXBnCTRL_T;

#define ABTF_MESSAGE_ABORTED 1u
#define ABTF_MESSAGE_TRANSMISSION_SUCCESSFUL 0
#define MLOA_MESSAGE_LOST_ARBITRATION 1u
#define TXERR_BUS_ERROR_DETECTED 1u
#define TXREQ_TX_START 1u
#define TXREQ_TX_STOP 0
#define TXP_PRIORITY_HIGH 1u
#define TXP_PRIORITY_LOW 0

#define TXB0CTRL_ADDR 0x30
TXBnCTRL_T TXB0CTRL;

#define TXB1CTRL_ADDR 0x40
TXBnCTRL_T TXB1CTRL;

#define TXB2CTRL_ADDR 0x50
TXBnCTRL_T TXB2CTRL;


/** ~TXnRTS Pin Control and Status Register */
typedef union
{
    struct {
        /** ~TX0RTS Pin Mode (R/W) */
        VBOOL_T B0RTSM      : 1;
        /** ~TX1RTS Pin Mode (R/W) */
        VBOOL_T B1RTSM      : 1;
        /** ~TX2RTS Pin Mode (R/W) */
        VBOOL_T B2RTSM      : 1;
        /** ~TX0RTS Pin State (R) */
        VBOOL_T B0RTS       : 1;
        /** ~TX1RTS Pin State (R) */
        VBOOL_T B1RTS       : 1;
        /** ~TX2RTS Pin State (R) */
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

/** Transmit Buffer n Standard Identifier Register, High Byte */
typedef union 
{
    struct {
        /** Standard Identifier bits */
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

/** Transmit Buffer n Standard Identifier Register, Low Byte */
typedef union 
{
    struct {
        /** Extended Identifier bits */
        VBOOL_T EID16       : 1;
        VBOOL_T EID17       : 1;
        VBOOL_T Reserved2   : 1;
        /** Extended Identifier Enable bit */
        VBOOL_T EXIDE       : 1;
        VBOOL_T Reserved4   : 1;
        /** Standard Identifier bits */
        VBOOL_T SID0        : 1;
        VBOOL_T SID1        : 1;
        VBOOL_T SID2        : 1;
    } bits;
    VU8_T byte;
} TXBnSIDL_T;

/** Configure the message identifier as a Standard 11-bit identifier. */
#define EXIDE_STANDARD_IDENTIFIER 0

/** Number of bits to left-shift an identifier to get the low bits into SIDL */
#define SIDL_SHIFT 5
/** Masks off only the Standard Identifier bits */
#define SIDL_MASK (U8_T) 0b11100000
/** Number of bits to right-shift an identifier to get the high bits */
#define SIDH_SHIFT 3
/** Masks off only the high bits, after shifting */
#define SIDH_MASK (U8_T) 0xFF
#define SID_GET_HIGH_BITS(i) (U8_T)((i >> SIDH_SHIFT) & SIDH_MASK)
#define SID_GET_LOW_BITS(i)  (U8_T)((i << SIDL_SHIFT) & SIDL_MASK)

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


/** Transmit Buffer n Data Length Code Register */
typedef union
{
    struct {
        /** Data Length Code bits */
        VBOOL_T DLC0        : 1;
        VBOOL_T DLC1        : 1;
        VBOOL_T DLC2        : 1;
        VBOOL_T DLC3        : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        /** Remote Transmission Request bit */
        VBOOL_T RTR         : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TXBnDLC_T;

/** Configure the next transmission as a Remote transmission (a request for
 * response). */
#define RTR_REMOTE_TRANSMISSION 1u
/** Configure the next transmission as a Data Frame. */
#define RTR_DATA_FRAME 0

/** Selects the bits that are relevant to the Data Length Control register. */
#define DLC_MASK 0b00001111


/** Address of the TXB0DLC register. */
#define TXB0DLC_ADDR 0x35
/** Data Length Control register 0. Represents the length of data in the CAN TX
 * B0 buffer. */
TXBnDLC_T TXB0DLC;

/** Address of the TXB0DLC register. */
#define TXB1DLC_ADDR 0x45
/** Data Length Control register 1. Represents the length of data in the CAN TX
 * B1 buffer. */
TXBnDLC_T TXB1DLC;

/** Address of the TXB2DLC register. */
#define TXB2DLC_ADDR 0x55
/** Data Length Control register 2. Represents the length of data in the CAN TX
 * B2 buffer. */
TXBnDLC_T TXB2DLC;


/** Can Control Register */
typedef union {
    struct {
        /** Clock Out Prescaler bits */
        VBOOL_T CLKPRE0 : 1;
        VBOOL_T CLKPRE1 : 1;
        /** Clock Out Enable bit */
        VBOOL_T CLKEN   : 1;
        /** One-Shot Mode bit */
        VBOOL_T OSM     : 1;
        /** Abort All Pending Transmissions bit */
        VBOOL_T ABAT    : 1;
        /** Request Operation Mode bits */
        VBOOL_T REQOP0  : 1;
        VBOOL_T REQOP1  : 1;
        VBOOL_T REQOP2  : 1;
    } bits;
    VU8_T byte;
} CANCTRL_T;

/** Address of the CANCTRL register. */
#define CANCTRL_ADDR 0x0F
/** Mode Control register value. */
CANCTRL_T CANCTRL;

/** Configure the MCP2515 in Normal operation mode, where messages can be sent 
 * and received. */
#define CANCTRL_MODE_NORMAL     0b00000000
/** Configure the MCP2515 as a loopback device. */
#define CANCTRL_MODE_LOOPBACK   0b00000010
/** Configure the MCP2515 in Configuration mode, where certain configuration bits
 * can be changed. */
#define CANCTRL_MODE_CONFIG     0b00000100

/** Base address of the TX B0 buffer data bytes. There are 8 data bytes following
 * this address. */
#define TXB0D_BASE_ADDR 0x36
/** Base address of the TX B1 buffer data bytes. There are 8 data bytes following
 * this address. */
#define TXB1D_BASE_ADDR 0x46
/** Base address of the TX B2 buffer data bytes. There are 8 data bytes following
 * this address. */
#define TXB2D_BASE_ADDR 0x56


/* CAN Messages/Commands/Instructions */

/** Perform a soft reset of the the MCP2515 device, initializing its registers 
 * to 0. */
#define CAN_CMD_RESET       0b11000000
/** Read from a register in the MCP2515 device. */
#define CAN_CMD_READ        0b00000011
/** Write to a register in the MCP2515 device. */
#define CAN_CMD_WRITE       0b00000010
#define CAN_CMD_READ_STATUS 0b10100000
#define CAN_CMD_RX_STATUS   0b10110000
#define CAN_CMD_BIT_MODIFY  0b00000101

/** Request To Send a particular buffer indicated by the lower 3 bits. */
#define CAN_CMD_RTS         0b10000000
/** Request To Send the B0 message buffer.
#define CAN_CMD_RTS_B0      0b00000001

/** Start writing directly into a data buffer as indicated by the lower bits. */
#define CAN_CMD_LOAD_TX                     0b01000000
/** Start writing directly into the B0 Standard Identifier bytes. */
#define CAN_CMD_LOAD_TX_START_AT_TXB0SIDH   0b00000000
/** Start writing directly into the B0 message data bytes. */
#define CAN_CMD_LOAD_TX_START_AT_TXB0D0     0b00000001


#endif