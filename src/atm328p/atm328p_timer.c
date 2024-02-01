#include "atm328p_timer.h"
#include "timer.h"

/*******************************************************************************
 * Enable the Timer 1 device hardware.
 * \param[in] prescale          The divisor to be used to configure the timer 
 *                              prescaler.
 * \param[in] interrupt_mode    Specifies whether timer interrupts are to be 
 *                              enabled or disabled.
 ******************************************************************************/
void timer1_enable(
    TIMER1_PRESCALE_T prescale, 
    ENABLE_T interrupt_mode
)
{
    /* Normal timer mode */
    TCCR1A.byte = TCCR1A_NORMAL_MODE;
    if (TIMER1_PRESCALE_OVER_256 == prescale) {
        TCCR1B.byte |= TCCR1B_PRESCALE_OVER_256;
    }
    if (TIMER1_INTERRUPT_ENABLED == interrupt_mode) {
        TIMSK1.bits.TOIE1 = TOIE1_ENABLE_INTERRUPT;
    } else {
        TIMSK1.bits.TOIE1 = TOIE1_DISABLE_INTERRUPT;
    }
}


/*******************************************************************************
 * Set the value of the Timer/Counter 1 to the given value.
 * \param[in] value The value to which the Timer/Counter 1 will be set.
 ******************************************************************************/
void timer1_set(U16_T value)
{
    TCNT1.halfword = value;
}