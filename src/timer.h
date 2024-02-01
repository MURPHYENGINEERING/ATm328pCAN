#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

/** Divisors for the Timer/Counter 1 prescaler. */
typedef enum {
    /** Timer/Counter 1 prescaler = `F_CLK` / 256. */
    TIMER1_PRESCALE_OVER_256
} TIMER1_PRESCALE_T;


/** Overflow interrupt mode for the Timer/Counter 1 */
typedef enum {
    /** Timer/Counter 1 will not initiate interrupts on overflow. */
    TIMER1_OVF_INTERRUPT_DISABLED,
    /** Timer/Counter 1 will initiate interrupts on overflow. */
    TIMER1_OVF_INTERRUPT_ENABLED
} TIMER1_OVF_INTERRUPT_MODE_T;


void timer1_enable(
    TIMER1_PRESCALE_T prescale, 
    TIMER1_OVF_INTERRUPT_MODE_T ovf_interrupt_mode
);

void timer1_set(U16_T value);


#endif