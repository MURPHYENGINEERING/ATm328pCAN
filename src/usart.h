#ifndef __USART_H__
#define __USART_H__

#include "types.h"


typedef enum {
    USART_MODE_ASYNCHRONOUS,
    USART_MODE_SYNCHRONOUS,
    USART_MODE_MASTERSPI
} USART_MODE_T;


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


typedef enum {
    USART_PARITY_MODE_NONE,
    USART_PARITY_MODE_EVEN,
    USART_PARITY_MODE_ODD
} USART_PARITY_MODE_T;


typedef enum {
    USART_CHARACTER_SIZE_5,
    USART_CHARACTER_SIZE_6,
    USART_CHARACTER_SIZE_7,
    USART_CHARACTER_SIZE_8,
    /* Not implemented
    USART_CHARACTER_SIZE_9
    */
} USART_CHARACTER_SIZE_T;


typedef enum {
    USART_STOP_BITS_1,
    USART_STOP_BITS_2
} USART_STOP_BITS_T;


typedef enum {
    USART_CLOCK_POLARITY_RISING_TRANSMITS,
    USART_CLOCK_POLARITY_FALLING_TRANSMITS
} USART_CLOCK_POLARITY_T;


typedef struct {
    USART_MODE_T mode;
    USART_BAUD_RATE_T baud;
    USART_CHARACTER_SIZE_T character_size;
    USART_STOP_BITS_T stop_bits;
    USART_PARITY_MODE_T parity;
    USART_CLOCK_POLARITY_T clock_polarity;
} USART_CONFIG_T;


typedef struct {
    BOOL_T parity_error;
    BOOL_T frame_error;
    BOOL_T overrun;
    SIZE_T len;
} USART_RX_RESULT_T;


typedef struct {
    SIZE_T len;
} USART_TX_RESULT_T;


void usart_init(USART_CONFIG_T cfg);
void usart_init_hardware(USART_CONFIG_T cfg);

void usart_tx_byte(U8_T data);
VU8_T usart_rx_byte(void);

SIZE_T usart_tx(U8_T* buf, SIZE_T len);
SIZE_T usart_rx(U8_T* buf, SIZE_T len);

BOOL_T usart_tx_ready(void);
BOOL_T usart_rx_pending(void);

BOOL_T usart_parity_error(void);
BOOL_T usart_data_overrun(void);
BOOL_T usart_frame_error(void);


#endif