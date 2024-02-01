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
    /** Timer/Counter 1 will initiate interrupts on overflow. */
    TIMER1_OVF_INTERRUPT_ENABLED = 1,
    /** Timer/Counter 1 will initiate interrupts on Comparison A match. */
    TIMER1_COMPA_INTERRUPT_ENABLED = 2,
    /** Timer/Counter 1 will initiate interrupts on Comparison B match. */
    TIMER1_COMPB_INTERRUPT_ENABLED = 4
} TIMER1_INTERRUPT_FLAGS_T;


/** Represents the comparator choices for Timer/Counter 1. */
typedef enum {
    /** Timer/Counter 1 Comparator A */
    TIMER1_COMP_A,
    /** Timer/Counter 1 Comparator B */
    TIMER1_COMP_B
} TIMER1_COMPARATOR_T;


/** Represents the value of Timer/Counter 1 */
typedef U16_T TIMER1_VALUE_T;


void timer1_enable(
    TIMER1_PRESCALE_T prescale, 
    TIMER1_INTERRUPT_FLAGS_T interrupt_flags
);

void timer1_set(TIMER1_VALUE_T value);


#endif