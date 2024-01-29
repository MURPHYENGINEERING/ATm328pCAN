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

#define WDE_WATCHDOG_DISABLED FALSE
#define WDE_WATCHDOG_ENABLED TRUE

/* Watchdog Timer Prescaler 3, 2, 1, 0 */
#define WDTCSR_WDP3 5
#define WDTCSR_WDP2 2
#define WDTCSR_WDP1 1
#define WDTCSR_WDP0 0
/* 256K WDT Oscillator Cycles before Timeout (2.0s at 5V) */
#define WDTCSR_PRESCALE_TIMEOUT_IN_2S \
    (U8_T) ( (1u << WDTCSR_WDP2) | (1u << WDTCSR_WDP1) | (1u << WDTCSR_WDP0) )

#endif