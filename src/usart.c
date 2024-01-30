#include "usart.h"
#include "interrupts.h"


U8_T g_usart_tx_buf[USART_BUF_LEN];
VSIZE_T g_usart_tx_buf_n;
VSIZE_T g_usart_tx_buf_write_idx;
VSIZE_T g_usart_tx_buf_read_idx;

VBOOL_T g_usart_tx_transmitting;

U8_T g_usart_rx_buf[USART_BUF_LEN];
VSIZE_T g_usart_rx_buf_write_idx;
VSIZE_T g_usart_rx_buf_read_idx;
VSIZE_T g_usart_rx_buf_n;


static void usart_tx_byte_from_buffer(void)
{
    U8_T c;

    if ((SIZE_T) 0 < g_usart_tx_buf_n) {
        c = g_usart_tx_buf[g_usart_tx_buf_read_idx];

        ++g_usart_tx_buf_read_idx;
        if (USART_BUF_LEN == g_usart_tx_buf_read_idx) {
            g_usart_tx_buf_read_idx = (SIZE_T) 0;
        }

        g_usart_tx_transmitting = TRUE;

        /* Set this last so we don't re-enter this interrupt while doing the above */
        UDR0 = c;
    } else {
        g_usart_tx_transmitting = FALSE;
    }
}


ISR(USART_TX_vect)
{
    usart_tx_byte_from_buffer();
}


ISR(USART_RX_VECT)
{
    U8_T c;

    if (FALSE == UCSR0A.bits.UPEn) {
        /* No parity error */
        c = UDR0.byte;
        
        if (g_usart_rx_buf_n < USART_BUF_LEN) {
            g_usart_rx_buf[g_usart_rx_buf_write_idx] = c;

            ++g_usart_rx_buf_n;
            ++g_usart_rx_buf_write_idx;
            if (USART_BUF_LEN == g_usart_rx_buf_write_idx) {
                g_usart_rx_buf_write_idx = (SIZE_T) 0;
            }
        } else {
            /* Buffer overflow, drop byte and report fault */
        }
    } else {
        /* Parity error, report fault */

        /* Read the byte to free the buffer */
        c = UDR0.byte;
    }
}


void usart_init(USART_CONFIG_T config)
{
    g_usart_rx_buf_write_idx    = (SIZE_T) 0;
    g_usart_rx_buf_read_idx     = (SIZE_T) 0;
    g_usart_rx_buf_n            = (SIZE_T) 0;

    g_usart_tx_buf_write_idx    = (SIZE_T) 0;
    g_usart_tx_buf_read_idx     = (SIZE_T) 0;
    g_usart_tx_buf_n            = (SIZE_T) 0;

    g_usart_tx_transmitting = FALSE;

    usart_init_hardware(config);
}


SIZE_T usart_tx(U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    /* Start buffering at the second index because we'll write out the first
     * one manually to get the transfer started.
     * Transfer will continue by interrupts after that. */
    i = (SIZE_T) 0;

    while ( (USART_BUF_LEN > i) && (i < len) ) {
        g_usart_tx_buf[g_usart_tx_buf_write_idx] = buf[i];

        ++g_usart_tx_buf_write_idx;
        if (USART_BUF_LEN == g_usart_tx_buf_write_idx) {
            g_usart_tx_buf_write_idx = (SIZE_T) 0;
        }

        ++i;
    }

    g_usart_tx_buf_n += i;

    if (FALSE == g_usart_tx_transmitting) {
        usart_tx_byte_from_buffer();
    }

    return i;
}


SIZE_T usart_rx(U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    i = (SIZE_T) 0;

    while ( (i < g_usart_rx_buf_n) && (i < len) ) {
        buf[i] = g_usart_rx_buf[g_usart_rx_buf_read_idx];
        
        ++g_usart_rx_buf_read_idx;
        if (USART_BUF_LEN == g_usart_rx_buf_read_idx) {
            g_usart_rx_buf_read_idx = (SIZE_T) 0;
        }

        ++i;
    }

    g_usart_rx_buf_n -= i;

    return i;
}