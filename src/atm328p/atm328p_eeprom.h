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

/** EEPROM Programming Mode bit 1 position */
#define EECR_EEPM1 5
/** EEPROM Programming Mode bit 0 position */
#define EECR_EEPM0 4
/** EEPROM Ready Interrupt Enable bit position */
#define EECR_EERIE 3
/** EEPROM Master Write Enable bit position */
#define EECR_EEMPE 2
/** EEPROM Write Enable bit position */
#define EECR_EEPE  1
/** Enable the Master Write bit and disable the Write bit while setting the
  * programming mode to erase-and-write and disabling interupts. */
#define EECR_PREPARE_ERASE_AND_WRITE \
    (U8_T)( (0u << EECR_EEPM0) | (0u << EECR_EEPM1) \
          | (0u << EECR_EERIE) | (1u << EECR_EEMPE) | (0u << EECR_EEPE) )
/** Enable the Write bit while setting the programming mode to erase-and-write
  * and disabling interrupts. Master Write stays enabled after 
  * `EECR_PREPARE_ERASE_AND_WRITE`.*/
#define EECR_EXECUTE_ERASE_AND_WRITE \
    (U8_T)( (0u << EECR_EEPM0) | (0u << EECR_EEPM1) \
          | (0u << EECR_EERIE) | (1u << EECR_EEMPE) | (1u << EECR_EEPE) )


#endif