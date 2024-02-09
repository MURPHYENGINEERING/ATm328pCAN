#include "atm328p_eeprom.h"
#include "atm328p_interrupts.h"

#include "eeprom.h"


/*******************************************************************************
 * Write a single given byte to the EEPROM at the given address.
 * \param[in] address   The address to which the byte will be written.
 * \param[in] data      The byte to be written.
 ******************************************************************************/
void eeprom_write_byte(SIZE_T address, U8_T data)
{
    cli();
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Set erase-and-write mode */
    EECR.bits.EEPM0 = FALSE;
    EECR.bits.EEPM1 = FALSE;

    /* Set up write */
    EEAR.halfword = (U16_T) address;
    EEDR.byte = data;

    /* These have to occur within 4 clock cycles of each other.
     * If we use the bit field then it may read, modify, write back, which will
     * take more than 4 cycles. */
    EECR.byte = EECR_PREPARE_ERASE_AND_WRITE;
    EECR.byte = EECR_EXECUTE_ERASE_AND_WRITE;
    sei();
}


/*******************************************************************************
 * Read a single byte from the EEPROM at the given address.
 * \param[in] address   The address from which the byte will be read.
 * \return The read byte.
 ******************************************************************************/
U8_T eeprom_read_byte(SIZE_T address)
{
    cli();
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Set up read */
    EEAR.halfword = (U16_T) address;
    /* Enable read */
    EECR.bits.EERE = TRUE;

    sei();

    return EEDR.byte;
}


/*******************************************************************************
 * Erase the byte in EEPROM at the given address (setting its value to 0).
 * \param[in] address The address at which the byte will be erased.
 ******************************************************************************/
void eeprom_erase_byte(SIZE_T address)
{
    /* I don't like that erasing writes 0xFF, so we write 0 instead. */
    eeprom_write_byte(address, (U8_T) 0u);
}