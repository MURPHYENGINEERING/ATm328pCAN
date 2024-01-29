#ifndef __ATM328P_MCU_H__
#define __ATM328P_MCU_H__

#include "types.h"


typedef union {
    struct {
        VBOOL_T PORF : 1;
        VBOOL_T EXTRF : 1;
        VBOOL_T BORF : 1;
        VBOOL_T WDRF : 1;
        VBOOL_T Reserved4 : 1;
        VBOOL_T Reserved5 : 1;
        VBOOL_T Reserved6 : 1;
        VBOOL_T Reserved7 : 1;
    } bits;
    VU8_T byte;
} MCUSR_T;

extern volatile MCUSR_T MCUSR;

#define WDRF_WATCHDOG_RESET_DISABLED FALSE

#endif