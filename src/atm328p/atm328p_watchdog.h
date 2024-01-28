#ifndef __ATM328P_WATCHDOG_H__
#define __ATM328P_WATCHDOG_H__

#include "types.h"


typedef union {
    struct {
        VBOOL_T WDP0 : 1;
        VBOOL_T WDP1 : 1;
        VBOOL_T WDP2 : 1;
        VBOOL_T WDE  : 1;
        VBOOL_T WDCE : 1;
        VBOOL_T WDP3 : 1;
        VBOOL_T WDIE : 1;
        VBOOL_T WDIF : 1;
    } bits;
    VU8_T byte;
} WDTCSR_T;

extern volatile WDTCSR_T WDTCSR;

#define WDTCSR_WATCHDOG_DISABLED (U8_T) 0
#define WDTCSR_WATCHDOG_ENABLED (U8_T) 1

#endif