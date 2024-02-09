#ifndef __ATM328P_EEPROM_H__
#define __ATM328P_EEPROM_H__

#include "types.h"

/** EEPROM Address Register (Low and High byte) */
extern volatile HALFWORD_T EEAR;

/** EEPROM Data Register */
extern volatile REGISTER_T EEDR;

/** EEPROM Control Register */
typedef union {
    struct {
        /** EEPROM Read Enable */
        VBOOL_T EERE        : 1;
        /** EEPROM Write Enable */
        VBOOL_T EEPE        : 1;
        /** EEPROM Master Write Enable */
        VBOOL_T EEMPE       : 1;
        /** EEPROM Ready Interrupt Enable */
        VBOOL_T EERIE       : 1;
        /** EEPROM Mode Bits */
        VBOOL_T EEPM0       : 1;
        VBOOL_T EEPM1       : 1;
        VBOOL_T Reserved6   : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} EECR_T;
/** EEPROM Control Register */
extern volatile EECR_T EECR;

#define EECR_EEMPE 2
#define EECR_EEPE  1
#define EECR_WRITE (U8_T)( (1u << EECR_EEMPE) | (1u << EECR_EEPE) )


#endif