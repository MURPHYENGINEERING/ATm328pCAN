#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "types.h"

void eeprom_write_byte(U8_T address, U8_T data);
U8_T eeprom_read_byte(U8_T address);
void eeprom_erase_byte(U8_T address);

void eeprom_write(U8_T base_address, U8_T* buf, SIZE_T len);
void eeprom_read(U8_T base_address, U8_T* buf, SIZE_T len);
void eeprom_erase(U8_T base_address, SIZE_T len);

#endif