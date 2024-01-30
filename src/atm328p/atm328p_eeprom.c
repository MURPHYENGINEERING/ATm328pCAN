#include "atm328p_eeprom.h"
#include "atm328p_interrupts.h"

#include "eeprom.h"


void eeprom_write_byte(SIZE_T address, U8_T data)
{
    cli();
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Set erase-and-write mode */
    EECR.bits.EEPM0 = TRUE;
    EECR.bits.EEPM1 = TRUE;

    /* Set up write */
    EEAR.byte = (U8_T) address;
    EEDR.byte = data;

    /* Enable master write */
    EECR.bits.EEMPE = TRUE;
    /* Enable write */
    EECR.bits.EEPE = TRUE;
    sei();
}


U8_T eeprom_read_byte(SIZE_T address)
{
    cli();
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Set up read */
    EEAR.byte = (U8_T) address;
    /* Enable read */
    EECR.bits.EERE = TRUE;

    sei();

    return EEDR.byte;
}


void eeprom_erase_byte(SIZE_T address)
{
<<<<<<< HEAD
    atm328p_eeprom_write_byte(address, (U8_T) 0);
=======
    /* Wait for previous write to complete */
    while (TRUE == EECR.bits.EEPE) {
    }

    /* Enable erase-only mode */
    EECR.bits.EEPM0 = TRUE;
    EECR.bits.EEPM1 = FALSE;

    /* Set up write */
    EEAR.byte = (U8_T) address;
    EEDR.byte = (U8_T) 0;

    /* Enable master write */
    EECR.bits.EEMPE = TRUE;
    /* Enable write */
    EECR.bits.EEPE = TRUE;
>>>>>>> refs/remotes/origin/main
}