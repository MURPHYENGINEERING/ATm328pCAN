#include "atm328p_timer.h"
#include "atm328p_mcu.h"
#include "atm328p_dsc.h"
#include "timer.h"
#include "fai.h"
#include "memory.h"


/*******************************************************************************
 * Enable the Timer 0 device hardware.
 * \param[in] mode              Specifies the timer mode (normal, PWM, etc.).
 * \param[in] prescale          The divisor to be used to configure the timer 
 *                              prescaler.
 * \param[in] pin_flags         Specifies the mode of the output pins.
 * \param[in] interrupt_flags   Specifies whether timer interrupts are to be 
 *                              enabled or disabled.
 ******************************************************************************/
void timer0_init(
    TIMER_MODE_T mode,
    TIMER0_PRESCALE_T prescale, 
    TIMER0_OUTPUT_PIN_FLAGS_T pin_flags,
    TIMER0_INTERRUPT_FLAGS_T interrupt_flags
)
{
    /* Disable power reduction */
    PRR.bits.PRTIM0 = PRTIM0_ENABLE_TIMER;

    /* Clear mode */
    TCCR0A.byte = (U8_T) 0;
    /* Disable prescaler */
    TCCR0B.byte = (U8_T) 0;
    /* Disable interrupts */
    TIMSK0.byte = (U8_T) 0;

    switch (mode) {
    case TIMER_MODE_NORMAL:
        TCCR0A.byte |= TCCR0A_MODE_NORMAL;
    break;
    case TIMER_MODE_CLEAR_ON_MATCH_A:
        TCCR0A.byte |= TCCR0A_MODE_CLEAR_ON_MATCH_A;
    break;
    case TIMER_MODE_FAST_PWM:
        TCCR0A.byte |= TCCR0A_MODE_FAST_PWM;
    break;
    case TIMER_MODE_PHASE_CORRECT_PWM:
        TCCR0A.byte |= TCCR0A_MODE_PHASE_CORRECT_PWM;
    break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
    break;
    }

    switch (prescale) {
    case TIMER0_OFF:                            TCCR0B.byte |= TCCR0B_CS_OFF;               break;
    case TIMER0_PRESCALE_OVER_1:                TCCR0B.byte |= TCCR0B_CS_OVER_1;            break;
    case TIMER0_PRESCALE_OVER_8:                TCCR0B.byte |= TCCR0B_CS_OVER_8;            break;
    case TIMER0_PRESCALE_OVER_64:               TCCR0B.byte |= TCCR0B_CS_OVER_64;           break;
    case TIMER0_PRESCALE_OVER_256:              TCCR0B.byte |= TCCR0B_CS_OVER_256;          break;
    case TIMER0_PRESCALE_OVER_1024:             TCCR0B.byte |= TCCR0B_CS_OVER_1024;         break;
    case TIMER0_EXTERNAL_CLOCK_RISING_EDGE:     TCCR0B.byte |= TCCR0B_CS_EXTERNAL_RISING;   break;
    case TIMER0_EXTERNAL_CLOCK_FALLING_EDGE:    TCCR0B.byte |= TCCR0B_CS_EXTERNAL_FALLING;  break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
    break;
    }

    if ( 0 != (pin_flags & TIMER0_OUTPUT_PIN_A_TOGGLE) ) {
        TCCR0A.bits.COM0A1 = TRUE;
        DDRD.bits.bit6 = TRUE;
    }

    if (0 != (interrupt_flags & TIMER0_OVF_INTERRUPT_ENABLED)) {
        TIMSK0.bits.TOIE0 = TIMSK_ENABLE_INTERRUPT;
    }

    if (0 != (interrupt_flags & TIMER0_COMPA_INTERRUPT_ENABLED)) {
        TIMSK0.bits.OCIE0A = TIMSK_ENABLE_INTERRUPT;
    }

    if (0 != (interrupt_flags & TIMER0_COMPB_INTERRUPT_ENABLED)) {
        TIMSK0.bits.OCIE0B = TIMSK_ENABLE_INTERRUPT;
    }
}

/*******************************************************************************
 * Enable the Timer 1 device hardware.
 * \param[in] mode              Specifies the timer mode (normal, PWM, etc.).
 * \param[in] prescale          The divisor to be used to configure the timer 
 *                              prescaler.
 * \param[in] pin_flags         Specifies the mode of the output pins.
 * \param[in] interrupt_flags   Specifies which timer interrupts are to be enabled.
 ******************************************************************************/
void timer1_init(
    TIMER_MODE_T mode,
    TIMER1_PRESCALE_T prescale, 
    TIMER1_OUTPUT_PIN_FLAGS_T pin_flags,
    TIMER1_INTERRUPT_FLAGS_T interrupt_flags
)
{
    /* Disable power reduction mode */
    PRR.bits.PRTIM1 = PRTIM1_ENABLE_TIMER;

    /* Clear mode */
    TCCR1A.byte = (U8_T) 0;
    /* Disable prescaler */
    TCCR1B.byte = (U8_T) 0;
    /* Disable interrupts */
    TIMSK1.byte = (U8_T) 0;

    switch (mode) {
    case TIMER_MODE_NORMAL:
        TCCR1A.byte |= TCCR1A_MODE_NORMAL;
    break;
    case TIMER_MODE_CLEAR_ON_MATCH_A:
        TCCR1B.byte |= TCCR1B_MODE_CLEAR_ON_MATCH_A;    
    break;
    case TIMER_MODE_FAST_PWM:
        TCCR1A.byte |= TCCR1A_MODE_FAST_PWM_OCR1A;
        TCCR1B.byte |= TCCR1B_MODE_FAST_PWM_OCR1A;    
    break;
    case TIMER_MODE_PHASE_CORRECT_PWM:
        TCCR1A.byte |= TCCR1A_MODE_PHASE_CORRECT_PWM_OCR1A; 
        TCCR1B.byte |= TCCR1B_MODE_PHASE_CORRECT_PWM_OCR1A;
    break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
    break;
    }

    switch (prescale) {
    case TIMER1_OFF:                            TCCR1B.byte |= TCCR1B_CS_OFF;               break;
    case TIMER1_PRESCALE_OVER_1:                TCCR1B.byte |= TCCR1B_CS_OVER_1;            break;
    case TIMER1_PRESCALE_OVER_8:                TCCR1B.byte |= TCCR1B_CS_OVER_8;            break;
    case TIMER1_PRESCALE_OVER_64:               TCCR1B.byte |= TCCR1B_CS_OVER_64;           break;
    case TIMER1_PRESCALE_OVER_256:              TCCR1B.byte |= TCCR1B_CS_OVER_256;          break;
    case TIMER1_PRESCALE_OVER_1024:             TCCR1B.byte |= TCCR1B_CS_OVER_1024;         break;
    case TIMER1_EXTERNAL_CLOCK_RISING_EDGE:     TCCR1B.byte |= TCCR1B_CS_EXTERNAL_RISING;   break;
    case TIMER1_EXTERNAL_CLOCK_FALLING_EDGE:    TCCR1B.byte |= TCCR1B_CS_EXTERNAL_FALLING;  break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
    }

    if ( 0 != (pin_flags & TIMER1_OUTPUT_PIN_A_TOGGLE) ) {
        TCCR1A.bits.COM1A1 = TRUE;
    }

    if ( 0 != (pin_flags & TIMER1_OUTPUT_PIN_B_TOGGLE) ) {
        TCCR1A.bits.COM1B1 = TRUE;
    }

    if ( 0 != (interrupt_flags & TIMER1_OVF_INTERRUPT_ENABLED) ) {
        TIMSK1.bits.TOIE1 = TIMSK_ENABLE_INTERRUPT;
    }

    if ( 0 != (interrupt_flags & TIMER1_COMPA_INTERRUPT_ENABLED) ) {
        TIMSK1.bits.OCIE1A = TIMSK_ENABLE_INTERRUPT;
    }

    if ( 0 != (interrupt_flags & TIMER1_COMPB_INTERRUPT_ENABLED) ) {
        TIMSK1.bits.OCIE1B = TIMSK_ENABLE_INTERRUPT;
    }
}


/*******************************************************************************
 * Set the value of the Timer/Counter 0 to the given value.
 * \param[in] value The value to which the Timer/Counter 0 will be set.
 ******************************************************************************/
void timer0_set(TIMER0_VALUE_T value)
{
    TCNT0.byte = (U8_T) value;
}


/*******************************************************************************
 * Set the value of the Timer/Counter 1 to the given value.
 * \param[in] value The value to which the Timer/Counter 1 will be set.
 ******************************************************************************/
void timer1_set(TIMER1_VALUE_T value)
{
    TCNT1.halfword = (U16_T) value;
}


/*******************************************************************************
 * Set the value of the Timer/Counter 1 Comparator to the given value.
 * \param[in] comp  The selected comparator to set.
 * \param[in] value The value to which the Timer/Counter 1 Comparator will be set.
 ******************************************************************************/
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
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FALSE, get_pc());
    break;
    }
}


/*******************************************************************************
 * Set the value of the Timer/Counter 0 Comparator to the given value.
 * \param[in] comp  The selected comparator to set.
 * \param[in] value The value to which the Timer/Counter 0 Comparator will be set.
 ******************************************************************************/
void timer0_set_comp(TIMER0_COMPARATOR_T comp, TIMER0_VALUE_T value)
{
    switch (comp) {
    case TIMER0_COMP_A:
        OCR0A.byte = (U8_T) value;
    break;

    case TIMER0_COMP_B:
        OCR0B.byte = (U8_T) value;
    break;

    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FALSE, get_pc());
    break;
    }
}


/*******************************************************************************
 * Set the PWM duty cycle for Timer/Counter 0.
 * PWM must be enabled by setting the timer mode in `timer0_init` to one of the
 * PWM modes.
 * \param[in] duty  The duty cycle at which to run PWM.
 ******************************************************************************/
void timer0_pwm(FLOAT_T duty)
{
    OCR0A.byte = (U8_T)( duty * (FLOAT_T)U8_T_MAX );
}


/*******************************************************************************
 * Set the PWM duty cycle for Timer/Counter 1.
 * PWM must be enabled by setting the timer mode in `timer1_init` to one of the
 * PWM modes.
 * \param[in] duty  The duty cycle at which to run PWM.
 ******************************************************************************/
void timer1_pwm(FLOAT_T duty)
{
    OCR1A.halfword = (U16_T)( duty * (FLOAT_T)U16_T_MAX );
}