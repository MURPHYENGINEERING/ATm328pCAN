#include "atm328p_timer.h"
#include "atm328p_mcu.h"
#include "timer.h"

/*******************************************************************************
 * Enable the Timer 0 device hardware.
 * \param[in] prescale          The divisor to be used to configure the timer 
 *                              prescaler.
 * \param[in] interrupt_mode    Specifies whether timer interrupts 
 *                              are to be enabled or disabled.
 ******************************************************************************/
void timer0_enable(
    TIMER1_PRESCALE_T prescale, 
    TIMER0_INTERRUPT_MODE_T interrupt_mode
)

/*******************************************************************************
 * Enable the Timer 1 device hardware.
 * \param[in] prescale          The divisor to be used to configure the timer 
 *                              prescaler.
 * \param[in] interrupt_flags   Specifies which timer interrupts are to be enabled.
 ******************************************************************************/
void timer1_enable(
    TIMER1_PRESCALE_T prescale, 
    TIMER1_INTERRUPT_FLAGS_T interrupt_flags
)
{
    /* Disable power reduction mode */
    PRR.bits.PRTIM1 = PRTIM1_ENABLE_TIMER;

    /* Normal timer mode */
    TCCR1A.byte = TCCR1A_NORMAL_MODE;

    switch (prescale) {
    case TIMER1_PRESCALE_OVER_256:
        TCCR1B.byte |= TCCR1B_PRESCALE_OVER_256;

    default:
        fai_pass_fail_logger(FAI_FAULT_ID_INVALID_TIMER_PRESCALE, FAIL, (U32_T) 0);    
    }

    if ( 0 == (interrupt_mode & TIMER1_OVF_INTERRUPT_ENABLED) ) {
        TIMSK1.bits.TOIE1 = TIMSK_DISABLE_INTERRUPT;
    } else {
        TIMSK1.bits.TOIE1 = TIMSK_ENABLE_INTERRUPT;
    }

    if ( 0 == (interrupt_mode & TIMER1_COMPA_INTERRUPT_ENABLED) ) {
        TIMSK1.bits.OCIE1A = TIMSK_DISABLE_INTERRUPT;
    } else {
        TIMSK1.bits.OCIE1A = TIMSK_ENABLE_INTERRUPT;
    }

    if ( 0 == (interrupt_mode & TIMER1_COMPB_INTERRUPT_ENABLED) ) {
        TIMSK1.bits.OCIE1B = TIMSK_DISABLE_INTERRUPT;
    } else {
        TIMSK1.bits.OCIE1B = TIMSK_ENABLE_INTERRUPT;
    }
}


/*******************************************************************************
 * Set the value of the Timer/Counter 1 to the given value.
 * \param[in] value The value to which the Timer/Counter 1 will be set.
 ******************************************************************************/
void timer1_set(TIMER1_VALUE_T value)
{
    TCNT1.halfword = (U16_T) value;
}


void timer1_set_comp(TIMER1_COMPARATOR_T comp, TIMER1_VALUE_T value)
{
    switch (comp) {
    case TIMER1_COMP_A:
        OCR1A.halfword = (U16_T) value;
    break;

    case TIMER1_COMP_B:
        OCR1B.halfword = (U16_T) value;
    break;

    default:
        fai_pass_fail_logger(FAI_FAULT_ID_INVALID_TIMER_COMPARATOR, FALSE, get_pc());
    break;
    }
}