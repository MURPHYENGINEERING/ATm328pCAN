#ifndef __ATM328P_USART_H__
#define __ATM328P_USART_H__

#include "types.h"


/** USART Control and Status Register n A */
typedef union {
    struct {
        /** Multi-processor Communication Mode (R/W) */
        VBOOL_T MCPMn : 1;
        /** Double the USART Transmission Speed (R/W) */
        VBOOL_T U2Xn : 1;
        /** USART Parity Error (R) */
        VBOOL_T UPEn : 1;
        /** Data OverRun (R) */
        VBOOL_T DORn : 1;
        /** Frame Error (R) */
        VBOOL_T FEn : 1;
        /** USART Data Register Empty (R) */
        VBOOL_T UDREn : 1;
        /** USART Transmit Complete (R/W) */
        VBOOL_T TXCn : 1;
        /** USART Receive Complete (R) */
        VBOOL_T RXCn : 1;
    } bits;
    VU8_T byte;
} UCSRnA_T;
/** USART Control and Status Register n A */
extern volatile UCSRnA_T UCSR0A;


/** USART Control and Status Register n B */
typedef union {
    struct {
        /** Transmit Data Bit 8 n (R/W) */
        VBOOL_T TXB8n : 1;
        /** Receive Data Bit 8 n (R) */
        VBOOL_T RXB8n : 1;
        /** Character Size n (R/W) */
        VBOOL_T UCSZn2 : 1;
        /** Transmitter Enable n (R/W) */
        VBOOL_T TXENn : 1;
        /** Receiver Enable n (R/W) */
        VBOOL_T RXENn : 1;
        /** USART Data Register Empty Interrupt Enable (R/W) */
        VBOOL_T UDRIEn : 1;
        /** TX Complete Interrupt Enable (R/W) */
        VBOOL_T TXCIEn : 1;
        /** RX Complete Interrupt Enable (R/W) */
        VBOOL_T RXCIEn : 1;
    } bits;
    VU8_T byte;
} UCSRnB_T;
/** USART Control and Status Register n B */
extern volatile UCSRnB_T UCSR0B;


/** USART Control and Status Register n C */
typedef union {
    struct {
        /** Clock Polarity (R/W) */
        VBOOL_T UCPOLn : 1;
        /** Character Size (R/W) */
        VBOOL_T UCSZn0 : 1;
        VBOOL_T UCSZn1 : 1;
        /** Stop Bit Select (R/W) */
        VBOOL_T USBSn : 1;
        /** Parity Mode (R/W) */
        VBOOL_T UPMn0 : 1;
        VBOOL_T UPMn1 : 1;
        /** USART Mode Select (R/W) */
        VBOOL_T UMSELn0 : 1;
        VBOOL_T UMSELn1 : 1;
    } bits;
    VU8_T byte;
} UCSRnC_T;
/** USART Control and Status Register n C */
extern volatile UCSRnC_T UCSR0C;


/** USART I/O Data Register 0 */
extern volatile REGISTER_T UDR0;


/** USART Baud Rate Registers */
extern volatile HALFWORD_T UBRR0;
#define UBRRn_MASK 0b0000111111111111u

/** USART Baud Rate Register values for fosc = 16-MHz */
#define UBRRn_2400bps    (U16_T) 416u
#define UBRRn_4800bps    (U16_T) 207u
#define UBRRn_9600bps    (U16_T) 103u
#define UBRRn_14400bps   (U16_T) 68u
#define UBRRn_19200bps   (U16_T) 51u
#define UBRRn_28800bps   (U16_T) 34u
#define UBRRn_38400bps   (U16_T) 25u
#define UBRRn_57600bps   (U16_T) 16u
#define UBRRn_76800bps   (U16_T) 12u
#define UBRRn_115200bps  (U16_T) 8u
#define UBRRn_230400bps  (U16_T) 3u
#define UBRRn_250000bps  (U16_T) 3u
#define UBRRn_500000bps  (U16_T) 1u
#define UBRRn_1000000bps (U16_T) 0u

#endif