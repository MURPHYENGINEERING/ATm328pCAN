#include "usart.h"


USART_TX_RESULT_T usart_tx_blocking(U8_T* buf, SIZE_T len)
{
    USART_TX_RESULT_T result;
    SIZE_T i;

    memset_by_U8((U8_T*)(void*)&result, 0, sizeof(result));

    for (i = 0; i < len; ++i) {
        usart_tx_byte(buf[i]);
    }

    result.len = len;
}


USART_TX_RESULT_T usart_tx_nonblocking(U8_T* buf, SIZE_T len)
{
    USART_TX_RESULT_T result;

    memset_by_U8((U8_T*)(void*)&result, 0, sizeof(result));

    if (usart_tx_ready()) {
        result = usart_tx_blocking(buf, len);
    }

    return result;
}


USART_RX_RESULT_T usart_rx_blocking(U8_T* buf, SIZE_T len)
{
    USART_RX_RESULT_T result;
    SIZE_T i;
    
    memset_by_U8((U8_T*)(void*)&result, 0, sizeof(result));

    for (i = 0; i < len; ++i) {
        /* Wait for a received byte so that parity and frame error will be set */
        while (FALSE == usart_rx_pending()) {
        }
        result.parity_error |= usart_parity_error();
        result.frame_error  |= usart_frame_error();
        result.overrun      |= usart_data_overrun();
        buf[i] = usart_rx_byte();
    }

    result.len = len;

    return result;
}


USART_RX_RESULT_T usart_rx_nonblocking(U8_T* buf, SIZE_T len)
{
    USART_RX_RESULT_T result;
    SIZE_T i;

    memset_by_U8((U8_T*)(void*)&result, 0, sizeof(result));

    i = 0;

    /* Bail out as soon as there's no data to read */
    while ((i < len) && usart_rx_pending()) {
        result.parity_error |= usart_parity_error();
        result.frame_error  |= usart_frame_error();
        result.overrun      |= usart_data_overrun();
        buf[i] = usart_rx_byte();

        ++i;
    }

    result.len = i;

    return result;
}