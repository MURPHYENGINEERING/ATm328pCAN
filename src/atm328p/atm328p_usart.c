#include "atm328p_usart.h"
#include "usart.h"


static void usart_set_mode(USART_MODE_T mode);
static void usart_set_baud(USART_BAUD_RATE_T baud);
static void usart_set_parity(USART_PARITY_MODE_T parity);
static void usart_set_stop_bits(USART_STOP_BITS_T stop_bits);
static void usart_set_character_size(USART_CHARACTER_SIZE_T character_size);
static void usart_set_clock_polarity(USART_CLOCK_POLARITY_T clock_polarity);


void usart_init_hardware(USART_CONFIG_T cfg)
{
    usart_set_mode(cfg.mode);
    usart_set_baud(cfg.baud);
    usart_set_parity(cfg.parity);
    usart_set_stop_bits(cfg.stop_bits);
    usart_set_character_size(cfg.character_size);

    if (USART_MODE_SYNCHRONOUS == cfg.mode) {
        /* This bit is used for synchronous mode only.
         * - ATmega328P Datasheet page 162 */
        usart_set_clock_polarity(cfg.clock_polarity);
    }

    /* Enable interrupts on RX and TX */
    UCSR0B.bits.RXCIEn = TRUE;
    UCSR0B.bits.TXCIEn = TRUE;
    UCSR0B.bits.UDRIEn = FALSE;

    /* Enable TX and RX */
    UCSR0B.bits.RXENn = TRUE;
    UCSR0B.bits.TXENn = TRUE;
}


void usart_tx_byte(U8_T data)
{
    /* Wait for the data buffer to be empty */
    while (FALSE == UCSR0A.bits.UDREn) {
    }

    UDR0.byte = data;
}


U8_T usart_rx_byte(void)
{
    /* Wait for a byte to become available */
    while (FALSE == UCSR0A.bits.RXCn) {
    }

    return UDR0.byte;
}


BOOL_T usart_parity_error(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.UPEn );
}


BOOL_T usart_data_overrun(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.DORn );
}


BOOL_T usart_frame_error(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.FEn );
}


BOOL_T usart_tx_ready(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.UDREn );
}


BOOL_T usart_rx_pending(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.RXCn );
}


static void usart_set_mode(USART_MODE_T mode)
{
    switch (mode)
    {
        case USART_MODE_ASYNCHRONOUS:
            UCSR0C.bits.UMSELn1 = FALSE;
            UCSR0C.bits.UMSELn0 = FALSE;
            /* This bit is used for synchronous mode only. 
             * Write this bit to zero when asynchronous mode is used.
             * - ATmega328P Datasheet page 162 */
            UCSR0C.bits.UCPOLn = FALSE;
        break;

        case USART_MODE_SYNCHRONOUS:
            UCSR0C.bits.UMSELn1 = FALSE;
            UCSR0C.bits.UMSELn0 = TRUE;
        break;

        case USART_MODE_MASTERSPI:
            UCSR0C.bits.UMSELn1 = TRUE;
            UCSR0C.bits.UMSELn0 = TRUE;
        break;

        default:
        /* S/W Error */
        break;
    }
}


static void usart_set_baud(USART_BAUD_RATE_T baud)
{
    switch (baud) {
    case USART_BAUD_2400:    UBRR0.halfword = (U16_T)( UBRRn_2400bps    & UBRRn_MASK ); break;
    case USART_BAUD_4800:    UBRR0.halfword = (U16_T)( UBRRn_4800bps    & UBRRn_MASK ); break;
    case USART_BAUD_9600:    UBRR0.halfword = (U16_T)( UBRRn_9600bps    & UBRRn_MASK ); break;
    case USART_BAUD_14400:   UBRR0.halfword = (U16_T)( UBRRn_14400bps   & UBRRn_MASK ); break;
    case USART_BAUD_19200:   UBRR0.halfword = (U16_T)( UBRRn_19200bps   & UBRRn_MASK ); break;
    case USART_BAUD_28800:   UBRR0.halfword = (U16_T)( UBRRn_28800bps   & UBRRn_MASK ); break;
    case USART_BAUD_38400:   UBRR0.halfword = (U16_T)( UBRRn_38400bps   & UBRRn_MASK ); break;
    case USART_BAUD_57600:   UBRR0.halfword = (U16_T)( UBRRn_57600bps   & UBRRn_MASK ); break;
    case USART_BAUD_76800:   UBRR0.halfword = (U16_T)( UBRRn_76800bps   & UBRRn_MASK ); break;
    case USART_BAUD_115200:  UBRR0.halfword = (U16_T)( UBRRn_115200bps  & UBRRn_MASK ); break;
    case USART_BAUD_230400:  UBRR0.halfword = (U16_T)( UBRRn_230400bps  & UBRRn_MASK ); break;
    case USART_BAUD_250000:  UBRR0.halfword = (U16_T)( UBRRn_250000bps  & UBRRn_MASK ); break;
    case USART_BAUD_500000:  UBRR0.halfword = (U16_T)( UBRRn_500000bps  & UBRRn_MASK ); break;
    case USART_BAUD_1000000: UBRR0.halfword = (U16_T)( UBRRn_1000000bps & UBRRn_MASK ); break;
    
    default:
        /* S/W Error */
        break;
    }
}


static void usart_set_parity(USART_PARITY_MODE_T parity)
{
    switch (parity)
    {
        case USART_PARITY_MODE_NONE:
            UCSR0C.bits.UPMn1 = FALSE;
            UCSR0C.bits.UPMn0 = FALSE;
        break;

        case USART_PARITY_MODE_EVEN:
            UCSR0C.bits.UPMn1 = TRUE;
            UCSR0C.bits.UPMn0 = FALSE;
        break;

        case USART_PARITY_MODE_ODD:
            UCSR0C.bits.UPMn1 = TRUE;
            UCSR0C.bits.UPMn0 = TRUE;
        break;

        default:
        /* S/W Error */
        break;
    }
}


static void usart_set_stop_bits(USART_STOP_BITS_T stop_bits)
{
    switch (stop_bits)
    {
        case USART_STOP_BITS_1: UCSR0C.bits.USBSn = FALSE; break;
        case USART_STOP_BITS_2: UCSR0C.bits.USBSn = TRUE; break;

        default:
        /* S/W Error */
        break;
    }
}


static void usart_set_character_size(USART_CHARACTER_SIZE_T character_size)
{
    switch (character_size)
    {
        case USART_CHARACTER_SIZE_5:
            UCSR0B.bits.UCSZn2 = FALSE;
            UCSR0C.bits.UCSZn1 = FALSE;
            UCSR0C.bits.UCSZn0 = FALSE;
        break;
        
        case USART_CHARACTER_SIZE_6:
            UCSR0B.bits.UCSZn2 = FALSE;
            UCSR0C.bits.UCSZn1 = FALSE;
            UCSR0C.bits.UCSZn0 = TRUE;
        break;
        
        case USART_CHARACTER_SIZE_7:
            UCSR0B.bits.UCSZn2 = FALSE;
            UCSR0C.bits.UCSZn1 = TRUE;
            UCSR0C.bits.UCSZn0 = FALSE;
        break;
        
        case USART_CHARACTER_SIZE_8:
            UCSR0B.bits.UCSZn2 = FALSE;
            UCSR0C.bits.UCSZn1 = TRUE;
            UCSR0C.bits.UCSZn0 = TRUE;
        break;
        
        /* Not implemented
        case USART_CHARACTER_SIZE_9:
            UCSR0B.bits.UCSZn2 = TRUE;
            UCSR0C.bits.UCSZn1 = TRUE;
            UCSR0C.bits.UCSZn0 = TRUE;
        break;
        */

        default:
        /* S/W Error */
        break;
    }
}


static void usart_set_clock_polarity(USART_CLOCK_POLARITY_T clock_polarity)
{
    switch (clock_polarity)
    {
        case USART_CLOCK_POLARITY_RISING_TRANSMITS:  UCSR0C.bits.UCPOLn = FALSE; break;
        case USART_CLOCK_POLARITY_FALLING_TRANSMITS: UCSR0C.bits.UCPOLn = TRUE; break;
             
        default:
        /* S/W Error */
        break;
    }
}