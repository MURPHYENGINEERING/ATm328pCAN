#include "eeprom.h"


/*******************************************************************************
 * Write the given array of bytes to the EEPROM, starting at the given base
 * address, up to the given length in bytes.
 * \param[in] base_address  The base address at which writing begins.
 * \param[in] p_buf         The byte array to be written to EEPROM.
 * \param[in] len           Length of the byte array to be written to EEPROM.
 ******************************************************************************/
void eeprom_write(void* base_address, U8_T* p_buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0u; i < len; ++i) {
        eeprom_write_byte((void*)( base_address + i ), p_buf[i] );
    }
}


/*******************************************************************************
 * Read into the given byte array from the EEPROM, starting at the given base
 * address, up to the given length in bytes.
 * \param[in] base_address  The base address at which reading begins.
 * \param[in] p_buf         The byte array into which the EEPROM will be read.
 * \param[in] len           Length of the byte array to be read from EEPROM. This
 *                          value is not range-checked; if `(base_address+len)`
 *                          exceeds the bounds of the EEPROM then behavior is
 *                          undefined.
 ******************************************************************************/
void eeprom_read(void* base_address, U8_T* p_buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0u; i < len; ++i) {
        p_buf[i] = eeprom_read_byte((void*)( base_address + i ));
    }
}


/*******************************************************************************
 * Erase the EEPROM, starting at the given base address, up to the given length 
 * in bytes.
 * \param[in] base_address  The base address at which erasing begins.
 * \param[in] len           The number of bytes to erase from the base address.
 ******************************************************************************/
void eeprom_erase(void* base_address, SIZE_T len)
{
    SIZE_T i;

    for (i = 0u; i < len; ++i) {
        eeprom_erase_byte((void*)( base_address + i ));
    }
}
