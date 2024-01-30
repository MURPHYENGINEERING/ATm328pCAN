#include "eeprom.h"


void eeprom_write(SIZE_T base_address, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        eeprom_write_byte((SIZE_T)( base_address + i, buf[i] ));
    }
}


void eeprom_read(SIZE_T base_address, U8_T* buf, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        buf[i] = eeprom_read_byte((SIZE_T)( base_address + i ));
    }
}


void eeprom_erase(SIZE_T base_address, SIZE_T len)
{
    SIZE_T i;

    for (i = 0; i < len; ++i) {
        eeprom_erase_byte((SIZE_T)( base_address + i ));
    }
}
