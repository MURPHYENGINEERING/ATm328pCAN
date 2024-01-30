#include "eeprom.h"
#include "atm328p/atm328p_eeprom.h"


void eeprom_write_byte(U8_T address, U8_T data)
{
    atm328p_eeprom_write_byte(address, data);
}


U8_T eeprom_read_byte(U8_T address)
{
    return atm328p_eeprom_read_byte(address);
}


void eeprom_erase_byte(U8_T address)
{
    atm328p_eeprom_erase_byte(address);
}


void eeprom_write(U8_T base_address, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        eeprom_write_byte(base_address + i, buf[i]);
    }
}


void eeprom_read(U8_T base_address, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        buf[i] = eeprom_read_byte(base_address + i);
    }
}


void eeprom_erase(U8_T base_address, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        eeprom_erase_byte(base_address + i);
    }
}
