#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

/** Divisors for the Timer/Counter 1 prescaler. */
typedef enum {
    /** Timer/Counter 1 prescaler off (no counting). */
    TIMER1_PRESCALE_OFF,
    /** Timer/Counter 1 prescaler = `F_CLK` / 1 (no prescale). */
    TIMER1_PRESCALE_OVER_1,
    /** Timer/Counter 1 prescaler = `F_CLK` / 8. */
    TIMER1_PRESCALE_OVER_8,
    /** Timer/Counter 1 prescaler = `F_CLK` / 64. */
    TIMER1_PRESCALE_OVER_64,
    /** Timer/Counter 1 prescaler = `F_CLK` / 256. */
    TIMER1_PRESCALE_OVER_256,
    /** Timer/Counter 1 prescaler = `F_CLK` / 1024. */
    TIMER1_PRESCALE_OVER_1024,
    /** Timer 1 counts on external clock on T1 pin, rising edge. */
    TIMER1_EXTERNAL_CLOCK_RISING_EDGE,
    /** Timer 1 counts on external clock on T1 pin, falling edge. */
    TIMER1_EXTERNAL_CLOCK_FALLING_EDGE
} TIMER1_PRESCALE_T;


/** Divisors for the Timer/Counter 0 prescaler. */
typedef enum {
    /** No prescaler (don't count) */
    TIMER0_OFF,
    /** Timer/Counter 0 prescaler = `F_CLK` / 1 (no prescale). */
    TIMER0_PRESCALE_OVER_1,
    /** Timer/Counter 0 prescaler = `F_CLK` / 8. */
    TIMER0_PRESCALE_OVER_8,
    /** Timer/Counter 0 prescaler = `F_CLK` / 64. */
    TIMER0_PRESCALE_OVER_64,
    /** Timer/Counter 0 prescaler = `F_CLK` / 256. */
    TIMER0_PRESCALE_OVER_256,
    /** Timer/Counter 0 prescaler = `F_CLK` / 1024. */
    TIMER0_PRESCALE_OVER_1024,
    /** Timer 0 counts on external clock on T0 pin, rising edge. */
    TIMER0_EXTERNAL_CLOCK_RISING_EDGE,
    /** Timer 0 counts on external clock on T0 pin, falling edge. */
    TIMER0_EXTERNAL_CLOCK_FALLING_EDGE
} TIMER0_PRESCALE_T;


/** Interrupt mode for the Timer/Counter 0 */
typedef enum {
    TIMER0_INTERRUPTS_OFF = 0,
    /** Timer/Counter 0 will initiate interrupts on overflow. */
    TIMER0_OVF_INTERRUPT_ENABLED = 1,
    /** Timer/Counter 0 will initiate interrupts on Comparison A match. */
    TIMER0_COMPA_INTERRUPT_ENABLED = 2,
    /** Timer/Counter 0 will initiate interrupts on Comparison B match. */
    TIMER0_COMPB_INTERRUPT_ENABLED = 4
} TIMER0_INTERRUPT_FLAGS_T;


/** Interrupt mode for the Timer/Counter 1 */
typedef enum {
    TIMER1_INTERRUPTS_OFF = 0,
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


/** Represents the comparator choices for Timer/Counter 0. */
typedef enum {
    /** Timer/Counter 0 Comparator A */
    TIMER0_COMP_A,
    /** Timer/Counter 0 Comparator B */
    TIMER0_COMP_B
} TIMER0_COMPARATOR_T;


/** Represents the value of Timer/Counter 1 */
typedef U16_T TIMER1_VALUE_T;
/** Represents the value of Timer/Counter 0 */
typedef U8_T TIMER0_VALUE_T;


/** Represents the modes that a PWM device can be configured in. */
typedef enum {
    /** The timer counts up to overflow and then resets to 0. */
    TIMER_MODE_NORMAL,
    /** The timer value is reset to 0 when it matches with Comparison A. */
    TIMER_MODE_CLEAR_ON_MATCH_A,
    /** The timer device is configured in Fast PWM mode, wherein it cycles once on each
      * clock. */
    TIMER_MODE_FAST_PWM,
    /** The timer device is configured in Phase Correct PWM mode, wherein it cycles
      * once per two clocks. */
    TIMER_MODE_PHASE_CORRECT_PWM
} TIMER_MODE_T;


void timer0_init(
    TIMER_MODE_T mode,
    TIMER0_PRESCALE_T prescale, 
    TIMER0_INTERRUPT_FLAGS_T interrupt_mode
);

void timer1_init(
    TIMER_MODE_T mode,
    TIMER1_PRESCALE_T prescale, 
    TIMER1_INTERRUPT_FLAGS_T interrupt_flags
);

void timer0_set(TIMER0_VALUE_T value);
void timer1_set(TIMER1_VALUE_T value);

void timer1_set_comp(TIMER1_COMPARATOR_T comp, TIMER1_VALUE_T value);
void timer0_set_comp(TIMER0_COMPARATOR_T comp, TIMER0_VALUE_T value);

void timer0_pwm(FLOAT_T duty);
void timer1_pwm(FLOAT_T duty);

#endif