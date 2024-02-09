#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "types.h"

void eeprom_write_byte(SIZE_T address, U8_T data);
U8_T eeprom_read_byte(SIZE_T address);
void eeprom_erase_byte(SIZE_T address);

void eeprom_write(SIZE_T base_address, U8_T* buf, SIZE_T len);
void eeprom_read(SIZE_T base_address, U8_T* buf, SIZE_T len);
void eeprom_erase(SIZE_T base_address, SIZE_T len);

#endif