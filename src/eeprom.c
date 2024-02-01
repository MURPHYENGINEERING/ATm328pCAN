#include "eeprom.h"


/*******************************************************************************
 * Write the given array of bytes to the EEPROM, starting at the given base
 * address, up to the given length in bytes.
 * \param[in] base_address  The base address at which writing begins.
 * \param[in] buf           The byte array to be written to EEPROM.
 * \param[in] len           Length of the byte array to be written to EEPROM.
 ******************************************************************************/
void eeprom_write(SIZE_T base_address, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        eeprom_write_byte((SIZE_T)( base_address + i ), buf[i] );
    }
}


/*******************************************************************************
 * Read into the given byte array from the EEPROM, starting at the given base
 * address, up to the given length in bytes.
 * \param[in] base_address  The base address at which reading begins.
 * \param[in] buf           The byte array into which the EEPROM will be read.
 * \param[in] len           Length of the byte array to be read from EEPROM. This
 *                          value is not range-checked; if `(base_address+len)`
 *                          exceeds the bounds of the EEPROM then behavior is
 *                          undefined.
 ******************************************************************************/
void eeprom_read(SIZE_T base_address, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        buf[i] = eeprom_read_byte((SIZE_T)( base_address + i ));
    }
}


/*******************************************************************************
 * Erase the EEPROM, starting at the given base address, up to the given length 
 * in bytes.
 * \param[in] base_address  The base address at which erasing begins.
 * \param[in] len           The number of bytes to erase from the base address.
 ******************************************************************************/
void eeprom_erase(SIZE_T base_address, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        eeprom_erase_byte((SIZE_T)( base_address + i ));
    }
}
