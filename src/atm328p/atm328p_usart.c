#include "atm328p_usart.h"
#include "atm328p_mcu.h"
#include "usart.h"


static void usart_set_mode(USART_MODE_T mode);
static void usart_set_baud(USART_BAUD_RATE_T baud);
static void usart_set_parity(USART_PARITY_MODE_T parity);
static void usart_set_stop_bits(USART_STOP_BITS_T stop_bits);
static void usart_set_character_size(USART_CHARACTER_SIZE_T character_size);
static void usart_set_clock_polarity(USART_CLOCK_POLARITY_T clock_polarity);


/*******************************************************************************
 * Initialize the USART hardware device with the given configuration parameters.
 * \param[in] cfg   The configuration parameters with which the hardware device
 *                  will be configured.
 ******************************************************************************/
void usart_init_hardware(USART_CFG_T cfg)
{
    /* Disable power reduction mode */
    PRR.bits.PRUSART0 = PRUSART0_ENABLE_USART;

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


/*******************************************************************************
 * Transmit a single byte to the USART hardware device by writing the data-out
 * register.
 * \param[in] data  The byte to be transmitted to the USART hardware device.
 ******************************************************************************/
void usart_tx_byte(U8_T data)
{
    UDR0.byte = data;
}


/*******************************************************************************
 * Receive a single byte from the USART hardware device by reading the data-in
 * register.
 * \return The byte read from the USART hardware device.
 ******************************************************************************/
U8_T usart_rx_byte(void)
{
    return UDR0.byte;
}


/*******************************************************************************
 * Get the state of the parity error bit from the USART hardware device by
 * reading the parity-error register.
 * \return The state of the parity error bit.
 ******************************************************************************/
BOOL_T usart_parity_error(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.UPEn );
}


/*******************************************************************************
 * Get the state of the data overrun error bit from the USART hardware device by
 * reading the data-overrun register.
 * \return The state of the data-overrun bit.
 ******************************************************************************/
BOOL_T usart_data_overrun(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.DORn );
}


/*******************************************************************************
 * Get the state of the frame error bit from the USART hardware device by
 * reading the frame-error register.
 * \return The state of the frame-error bit.
 ******************************************************************************/
BOOL_T usart_frame_error(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.FEn );
}


/*******************************************************************************
 * Get the state of the transmitter from the USART hardware device by reading
 * the state of the TX-data-empty register.
 * \return The state of the TX-data-empty bit.
 ******************************************************************************/
BOOL_T usart_tx_ready(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.UDREn );
}


/*******************************************************************************
 * Get the state of the receiver from the USART hardware device by reading the
 * state of the RX-data-ready register.
 * \return The state of the RX-data-ready bit.
 ******************************************************************************/
BOOL_T usart_rx_pending(void)
{
    return (BOOL_T)( TRUE == UCSR0A.bits.RXCn );
}


/*******************************************************************************
 * Configure the mode of the USART hardware device by setting the appropriate
 * mode registers.
 * \param[in] mode  The given mode to which the USART hardware device will be set.
 ******************************************************************************/
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


/*******************************************************************************
 * Set the baud rate of the USART hardware device by setting the appropriate
 * register.
 * \param[in] baud The baud rate to be set on the hardware device.
 ******************************************************************************/
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


/*******************************************************************************
 * Set the parity mode of the USART hardware device by setting the appropriate
 * register.
 * \param[in] parity The parity mode to be set on the hardware device.
 ******************************************************************************/
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


/*******************************************************************************
 * Set the number of stop bits on the USART hardware device by setting the
 * appropriate register.
 * \param[in] stop_bits The number of stop bits to be set on the hardware device.
 ******************************************************************************/
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


/*******************************************************************************
 * Set the character size on the USART hardware device by setting the appropriate
 * register.
 * \param[in] character_size The character size to be set on the hardware device.
 ******************************************************************************/
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


/*******************************************************************************
 * Set the clock polarity of the USART hardware device by setting the appropriate 
 * register.
 * \param[in] clock_polarity The clock polarity to be set on the hardware device.
 ******************************************************************************/
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