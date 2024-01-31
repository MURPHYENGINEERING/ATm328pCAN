#include "usart.h"
#include "interrupts.h"
#include "dsc.h"

#define USART_BUF_LEN 256

U8_T g_usart_tx_buf[USART_BUF_LEN];
VSIZE_T g_usart_tx_buf_n;
VSIZE_T g_usart_tx_buf_write_idx;
VSIZE_T g_usart_tx_buf_read_idx;

/* True if we're currently driving bytes out of the buffer by interrupts.
 * False when we run out of bytes and the interrupt cycle stops.
 * This dictates whether we should initiate interrupts by manually driving a
 * byte out of the buffer after writing to it. */
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

        g_usart_tx_buf_read_idx = 
            (SIZE_T)(g_usart_tx_buf_read_idx + 1u) % USART_BUF_LEN;

        --g_usart_tx_buf_n;

        g_usart_tx_transmitting = TRUE;

        /* Do this last so we don't re-enter this interrupt */
        usart_tx_byte(c);
    } else {
        g_usart_tx_transmitting = FALSE;
    }
}


ISR(USART_TX_vect)
{
    usart_tx_byte_from_buffer();
}


ISR(USART_RX_vect)
{
    U8_T c;

    if (FALSE == usart_parity_error()) {
        /* Read the byte to disable the interrupt */
        c = usart_rx_byte();

        if (USART_BUF_LEN > g_usart_rx_buf_n) {
            g_usart_rx_buf[g_usart_rx_buf_write_idx] = c;

            g_usart_rx_buf_write_idx = 
                (SIZE_T)(g_usart_rx_buf_write_idx + 1u) % USART_BUF_LEN;

            ++g_usart_rx_buf_n;
        } else {
            /* Buffer overflow, byte is lost. */
            fai_pass_fail_logger(
                FAI_FAULT_ID_USART_RX_BUFFER_OVERFLOW, 
                FAIL, 
                (U32_T) c
            );
        }
    } else {
        /* Read the byte to disable the interrupt */
        c = usart_rx_byte();
        /* Report a Parity fault */
        fai_pass_fail_logger(FAI_FAULT_ID_USART_PARITY_ERROR, FAIL, (U32_T) c);
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

    i = (SIZE_T) 0;

    while ( (USART_BUF_LEN > i) && (i < len) ) {
        g_usart_tx_buf[g_usart_tx_buf_write_idx] = buf[i];

        g_usart_tx_buf_write_idx = 
            (SIZE_T)(g_usart_tx_buf_write_idx + 1u) % USART_BUF_LEN;

        ++i;
    }

    g_usart_tx_buf_n += i;

    if (FALSE == g_usart_tx_transmitting) {
        /* Put the busy-wait here because none of the underlying functions do
         * it (because they run in the ISR) */
        while (!usart_tx_ready()) {
        }
        /* Kick off the transmission; it will be interrupt-driven after that. */
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
        
        g_usart_rx_buf_read_idx =
            (SIZE_T)(g_usart_rx_buf_read_idx + 1u) % USART_BUF_LEN;

        ++i;
    }

    g_usart_rx_buf_n -= i;

    return i;
}