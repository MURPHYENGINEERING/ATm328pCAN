#ifndef __USART_H__
#define __USART_H__

#include "types.h"


/** Represents the operating mode of the USART device. */
typedef enum {
    /** The USART devices operates in Asynchronous mode with no clock. */
    USART_MODE_ASYNCHRONOUS,
    /** The USART device operates in Synchronous mode with a synchronizing clock 
      * pulse. */
    USART_MODE_SYNCHRONOUS,
    /** The USART device operates in Master SPI mode (3-wire). */
    USART_MODE_MASTERSPI
} USART_MODE_T;


/** Represents the baud rate (bits per second) of the USART device. */
typedef enum {
    USART_BAUD_2400,
    USART_BAUD_4800,
    USART_BAUD_9600,
    USART_BAUD_14400,
    USART_BAUD_19200,
    USART_BAUD_28800,
    USART_BAUD_38400,
    USART_BAUD_57600,
    USART_BAUD_76800,
    USART_BAUD_115200,
    USART_BAUD_230400,
    USART_BAUD_250000,
    USART_BAUD_500000,
    USART_BAUD_1000000
} USART_BAUD_RATE_T;


/** Represents the Parity mode of the USART device. */
typedef enum {
    USART_PARITY_MODE_NONE,
    USART_PARITY_MODE_EVEN,
    USART_PARITY_MODE_ODD
} USART_PARITY_MODE_T;


/** Represents the character size (data length) of a USART word. */
typedef enum {
    USART_CHARACTER_SIZE_5,
    USART_CHARACTER_SIZE_6,
    USART_CHARACTER_SIZE_7,
    USART_CHARACTER_SIZE_8,
    /* Not implemented
    USART_CHARACTER_SIZE_9
    */
} USART_CHARACTER_SIZE_T;


/** Represents the number of stop bits appending a USART word. */
typedef enum {
    USART_STOP_BITS_1,
    USART_STOP_BITS_2
} USART_STOP_BITS_T;


/** Represents the polarity of the synchronizing clock in synchronous mode. */
typedef enum {
    USART_CLOCK_POLARITY_RISING_TRANSMITS,
    USART_CLOCK_POLARITY_FALLING_TRANSMITS
} USART_CLOCK_POLARITY_T;


/** Represents all the USART device hardware parameters. */
typedef struct {
    USART_MODE_T mode;
    USART_BAUD_RATE_T baud;
    USART_CHARACTER_SIZE_T character_size;
    USART_STOP_BITS_T stop_bits;
    USART_PARITY_MODE_T parity;
    USART_CLOCK_POLARITY_T clock_polarity;
} USART_CFG_T;


void usart_init(USART_CFG_T cfg);
void usart_init_hardware(USART_CFG_T cfg);

void usart_tx_byte(U8_T data);
U8_T usart_rx_byte(void);

SIZE_T usart_tx(U8_T* buf, SIZE_T len);
SIZE_T usart_rx(U8_T* buf, SIZE_T len);

BOOL_T usart_tx_ready(void);
BOOL_T usart_rx_pending(void);

BOOL_T usart_parity_error(void);
BOOL_T usart_data_overrun(void);
BOOL_T usart_frame_error(void);


#endif