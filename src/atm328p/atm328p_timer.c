#include "atm328p_timer.h"
#include "timer.h"

void timer1_enable(
    TIMER1_PRESCALE_T prescale, 
    TIMER1_INTERRUPT_MODE_T interrupt_mode
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


void timer1_set(U16_T value)
{
    TCNT1.halfword = value;
}