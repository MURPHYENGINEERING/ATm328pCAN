#ifndef __ATM328P_EEPROM_H__
#define __ATM328P_EEPROM_H__

#include "types.h"

/* EEPROM Address Register (Low Byte) */
typedef union {
    struct {
        VBOOL_T EEAR0 : 1;
        VBOOL_T EEAR1 : 1;
        VBOOL_T EEAR2 : 1;
        VBOOL_T EEAR3 : 1;
        VBOOL_T EEAR4 : 1;
        VBOOL_T EEAR5 : 1;
        VBOOL_T EEAR6 : 1;
        VBOOL_T EEAR7 : 1;
    } bits;
    VU8_T byte;
} EEAR_T;

extern volatile EEAR_T EEAR;

/* EEPROM Data Register */
extern volatile REGISTER_T EEDR;

/* EEPROM Control Register */
typedef union {
    struct {
        /* EEPROM Read Enable */
        VBOOL_T EERE : 1;
        /* EEPROM Write Enable */
        VBOOL_T EEPE : 1;
        /* EEPROM Master Write Enable */
        VBOOL_T EEMPE : 1;
        /* EEPROM Ready Interrupt Enable */
        VBOOL_T EERIE : 1;
        /* EEPROM Mode Bits
         * 0, 0 = Erase and write in one operation (3.4ms)
         * 0, 1 = Erase only (1.8ms)
         * 1, 0 = Write only (1.8ms)
         * 1, 1 = Reserved for future use */
        VBOOL_T EEPM0 : 1;
        VBOOL_T EEPM1 : 1;
        VBOOL_T Reserved6 : 1;
        VBOOL_T Reserved7 : 1;
    } bits;
    VU8_T byte;
} EECR_T;

extern volatile EECR_T EECR;

void atm328p_eeprom_write_byte(U8_T address, U8_T data);
U8_T atm328p_eeprom_read_byte(U8_T address);
void atm328p_eeprom_erase_byte(U8_T address);


#endif