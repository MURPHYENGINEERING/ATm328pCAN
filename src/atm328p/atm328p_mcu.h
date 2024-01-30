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


/* Power Reduction Register */
typedef union {
    struct {
        VBOOL_T PRADC : 1;
        VBOOL_T PRUSART0 : 1;
        /* Power Reduction Serial Peripheral Interface */
        VBOOL_T PRSPI : 1;
        VBOOL_T PRTIM1 : 1;
        VBOOL_T Reserved4 : 1;
        VBOOL_T PRTIM0 : 1;
        VBOOL_T PRTIM2 : 1;
        VBOOL_T PRTWI : 1;
    } bits;
    VU8_T byte;
} PRR_T;

extern volatile PRR_T PRR;

#define PRSPI_DISABLE_POWER_REDUCTION FALSE
#define PRSPI_ENABLE_POWER_REDUCTION TRUE

#define PRUSART0_DISABLE_POWER_REDUCTION FALSE

#endif