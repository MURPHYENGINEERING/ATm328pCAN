#ifndef __ATM328P_WATCHDOG_H__
#define __ATM328P_WATCHDOG_H__

#include "types.h"


/** Watchdog Control and Status Register */
typedef union {
    struct {
        /** Watchdog Prescaler bits */
        VBOOL_T WDP0 : 1;
        VBOOL_T WDP1 : 1;
        VBOOL_T WDP2 : 1;
        /** Watchdog Enable bit */
        VBOOL_T WDE  : 1;
        /** Watchdog Change Enable bit */
        VBOOL_T WDCE : 1;
        /** Watchdog Prescaler bit */
        VBOOL_T WDP3 : 1;
        /** Watchdog Enable Interrupts bit */
        VBOOL_T WDIE : 1;
        /** Watchdog Interrupt Flag bit */
        VBOOL_T WDIF : 1;
    } bits;
    VU8_T byte;
} WDTCSR_T;

/** Watchdog Control and Status Register */
extern volatile WDTCSR_T WDTCSR;

#define WDE_WATCHDOG_DISABLED FALSE
#define WDE_WATCHDOG_ENABLED TRUE

#define WDCE_CHANGE_WATCHDOG TRUE

/* Watchdog Timer Prescaler 3, 2, 1, 0 */
#define WDTCSR_WDP3 5
#define WDTCSR_WDE 3
#define WDTCSR_WDCE 4
#define WDTCSR_WDP2 2
#define WDTCSR_WDP1 1
#define WDTCSR_WDP0 0
/** 256K WDT Oscillator Cycles before Timeout (2.0s at 5V) */
#define WDTCSR_PRESCALE_TIMEOUT_IN_2S \
    (U8_T) ( (1u << WDTCSR_WDP2) | (1u << WDTCSR_WDP1) | (1u << WDTCSR_WDP0) )

#define WDTCSR_ENABLE_WATCHDOG (U8_T) (1u << WDTCSR_WDE)
#define WDTCSR_CHANGE_WATCHDOG (U8_T) (1u << WDTCSR_WDCE)


/** Reset the watchdog timer/counter to 0, preventing watchdog reset. */
#define watchdog_reset() __asm__ __volatile__ ("wdr")


#endif