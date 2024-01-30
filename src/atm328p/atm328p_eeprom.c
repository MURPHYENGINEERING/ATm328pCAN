#include "atm328p_eeprom.h"
#include "atm328p_interrupts.h"


void atm328p_eeprom_write_byte(U8_T address, U8_T data)
{
    cli();
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Set erase-and-write mode */
    EECR.bits.EEPM0 = TRUE;
    EECR.bits.EEPM1 = TRUE;

    /* Set up write */
    EEAR.byte = address;
    EEDR.byte = data;

    /* Enable master write */
    EECR.bits.EEMPE = TRUE;
    /* Enable write */
    EECR.bits.EEPE = TRUE;
    sei();
}


U8_T atm328p_eeprom_read_byte(U8_T address)
{
    cli();
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Set up read */
    EEAR.byte = address;
    /* Enable read */
    EECR.bits.EERE = TRUE;

    sei();

    return EEDR.byte;
}


void atm328p_eeprom_erase_byte(U8_T address)
{
    atm328p_eeprom_write_byte(address, (U8_T) 0);
}