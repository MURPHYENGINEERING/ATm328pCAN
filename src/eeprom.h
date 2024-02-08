#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "types.h"

void eeprom_write_byte(void* address, U8_T data);
U8_T eeprom_read_byte(void* address);
void eeprom_erase_byte(void* address);

void eeprom_write(void* base_address, U8_T* buf, SIZE_T len);
void eeprom_read(void* base_address, U8_T* buf, SIZE_T len);
void eeprom_erase(void* base_address, SIZE_T len);

#endif