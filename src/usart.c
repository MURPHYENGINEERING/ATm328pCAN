#include "usart.h"
#include "interrupts.h"
#include "dsc.h"
#include "fai.h"

#define USART_BUF_LEN 256u

VU8_T g_usart_tx_buf[USART_BUF_LEN];
VSIZE_T g_usart_tx_buf_n;
VSIZE_T g_usart_tx_buf_write_idx;
VSIZE_T g_usart_tx_buf_read_idx;

/* True if we're currently driving bytes out of the buffer by interrupts.
 * False when we run out of bytes and the interrupt cycle stops.
 * This dictates whether we should initiate interrupts by manually driving a
 * byte out of the buffer after writing to it. */
VBOOL_T g_usart_tx_transmitting;

VU8_T g_usart_rx_buf[USART_BUF_LEN];
VSIZE_T g_usart_rx_buf_write_idx;
VSIZE_T g_usart_rx_buf_read_idx;
VSIZE_T g_usart_rx_buf_n;


/*******************************************************************************
 * Transmit a byte from the USART TX buffer to the USART hardware device.
 ******************************************************************************/
static void usart_tx_byte_from_buffer(void)
{
    U8_T c;

    /* Disable interrupts so we don't get a TX interrupt while manipulating the
     * buffer. */
    cli();

    if ((SIZE_T) 0 < g_usart_tx_buf_n) {
        c = g_usart_tx_buf[g_usart_tx_buf_read_idx];

        g_usart_tx_buf_read_idx = 
            (SIZE_T)( (g_usart_tx_buf_read_idx + 1u) % USART_BUF_LEN );

        --g_usart_tx_buf_n;

        g_usart_tx_transmitting = TRUE;

        usart_tx_byte(c);
    } else {
        g_usart_tx_transmitting = FALSE;
    }

    sei();
}


/*******************************************************************************
 * Transmit a byte from the TX buffer when the TX register is ready for the next
 * byte.
 ******************************************************************************/
ISR(USART0_TX_vect)
{
    usart_tx_byte_from_buffer();
}


/*******************************************************************************
 * Receive a byte from the USART hardware and store it in the RX buffer.
 * This function logs a fault if the RX buffer is full and the incoming byte
 * is lost.
 ******************************************************************************/
ISR(USART0_RX_vect)
{
    U8_T c;

    /* Disable interupts so we don't get an RX interrupt while manipulating the
     * buffer. */
    cli();

    if (FALSE == usart_parity_error()) {
        /* Read the byte to disable the interrupt */
        c = usart_rx_byte();

        if (USART_BUF_LEN > g_usart_rx_buf_n) {
            g_usart_rx_buf[g_usart_rx_buf_write_idx] = c;

            g_usart_rx_buf_write_idx = 
                (SIZE_T)( (g_usart_rx_buf_write_idx + 1u) % USART_BUF_LEN );

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

    sei();
}


/*******************************************************************************
 * Initialize the USART software and hardware device by setting all buffers to
 * zero and çalling the appropriate hardware initializer.
 ******************************************************************************/
void usart_init(USART_CFG_T cfg)
{
    g_usart_rx_buf_write_idx    = (SIZE_T) 0u;
    g_usart_rx_buf_read_idx     = (SIZE_T) 0u;
    g_usart_rx_buf_n            = (SIZE_T) 0u;

    g_usart_tx_buf_write_idx    = (SIZE_T) 0u;
    g_usart_tx_buf_read_idx     = (SIZE_T) 0u;
    g_usart_tx_buf_n            = (SIZE_T) 0u;

    g_usart_tx_transmitting = FALSE;

    usart_init_hardware(cfg);
}


/*******************************************************************************
 * Write the given buffer to the USART TX buffer to be transmitted byte-by-byte
 * as the hardware becomes available.
 * This function starts writing if the hardware is not currently in a write cycle.
 * \param[in] buf The data to be written to the TX buffer.
 * \param[in] len The length of the data to be written to the TX buffer.
 * \return  The number of bytes actually written to the buffer. This may be less
 *          than `len` if the TX buffer becomes full.
 ******************************************************************************/
SIZE_T usart_tx(U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    /* Disable interrupts so we don't get a TX interrupt while manipulating the
     * buffer. */
    cli();

    i = (SIZE_T) 0u;

    while ( (USART_BUF_LEN > i) && (i < len) ) {
        g_usart_tx_buf[g_usart_tx_buf_write_idx] = buf[i];

        g_usart_tx_buf_write_idx = 
            (SIZE_T)( (g_usart_tx_buf_write_idx + 1u) % USART_BUF_LEN );

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

    sei();

    return i;
}


/*******************************************************************************
 * Receive data from the USART RX buffer into the given buffer, up to the given
 * number of bytes. 
 * \param[out] buf  The buffer into which the read data will be written.
 * \param[in] len   The number of bytes to be read from the RX buffer.
 * \return  The number of bytes actually read from the RX buffer. This may be
 *          less than `len` if the RX buffer becomes empty.
 ******************************************************************************/
SIZE_T usart_rx(U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    /* Disable interrupts so we don't get an RX interrupt while manipulating the
     * buffer. */
    cli();

    i = (SIZE_T) 0u;

    while ( (i < g_usart_rx_buf_n) && (i < len) ) {
        buf[i] = g_usart_rx_buf[g_usart_rx_buf_read_idx];
        
        g_usart_rx_buf_read_idx =
            (SIZE_T)( (g_usart_rx_buf_read_idx + 1u) % USART_BUF_LEN );

        ++i;
    }

    g_usart_rx_buf_n -= i;

    sei();

    return i;
}