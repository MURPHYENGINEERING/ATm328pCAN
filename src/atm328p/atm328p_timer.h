#ifndef __ATM328P_TIMER_H__
#define __ATM328P_TIMER_H__

#include "types.h"

/** Timer/Counter 0 Control Register A */
typedef union {
    struct {
        /** Waveform Generation Mode */
        VBOOL_T WGM00       : 1;
        VBOOL_T WGM01       : 1;
        VBOOL_T Reserved2   : 1;
        VBOOL_T Reserved3   : 1;
        /** Compare Match Output B Mode */ 
        VBOOL_T COM0B0      : 1;
        VBOOL_T COM0B1      : 1;
        /** Compare Match Output A Mode */ 
        VBOOL_T COM0A0      : 1;
        VBOOL_T COM0A1      : 1;
    } bits;
    VU8_T byte;
} TCCR0A_T;
/** Timer/Counter 0 Control Register A */
extern volatile TCCR0A_T TCCR0A;

#define TCCR0A_MODE_NORMAL              (U8_T) 0u
#define TCCR0A_MODE_PHASE_CORRECT_PWM   (U8_T) 0b00000001u
#define TCCR0A_MODE_CLEAR_ON_MATCH_A    (U8_T) 0b00000010u
#define TCCR0A_MODE_FAST_PWM            (U8_T) 0b00000011u

/** Timer/Counter 0 Control Register B */
typedef union {
    struct {
        /** Clock Select */
        VBOOL_T CS00        : 1;
        VBOOL_T CS01        : 1;
        VBOOL_T CS02        : 1;
        /** Waveform Generation Mode */
        VBOOL_T WGM02       : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        /** Force Output Compare B */
        VBOOL_T FOC0B       : 1;
        /** Force Output Compare A */
        VBOOL_T FOC0A       : 1;
    } bits;
    VU8_T byte;
} TCCR0B_T;
/** Timer/Counter 0 Control Register B */
extern volatile TCCR0B_T TCCR0B;

#define WGM02_PWM_ON_OCRA FALSE

/** Clock Select Bit 0 */
#define TCCR0B_CS00 0
/** Clock Select Bit 1 */
#define TCCR0B_CS01 1
/** Clock Select Bit 2 */
#define TCCR0B_CS02 2
/** Select the bits for the Clock Select in TCCR0B */
#define TCCR0B_CS_MASK \
    (U8_T)( (1u << TCCR0B_CS02) | (1u << TCCR0B_CS01) | (1u << TCCR0B_CS00) )
/** Select a prescale value of 0 (no counting) */
#define TCCR0B_CS_OFF (U8_T) 0u
/** Select a prescale value of FCLK/1 */
#define TCCR0B_CS_OVER_1 (U8_T)( 1u << TCCR0B_CS00 )
/** Select a prescale value of FCLK/8 */
#define TCCR0B_CS_OVER_8 (U8_T)( 1u << TCCR0B_CS01 )
/** Select a prescale value of FCLK/64 */
#define TCCR0B_CS_OVER_64 (U8_T)( (1u << TCCR0B_CS01) | (1u << TCCR0B_CS00) )
/** Select a prescale value of FCLK/256 */
#define TCCR0B_CS_OVER_256 (U8_T)( 1u << TCCR0B_CS02 )
/** Select a prescale value of FCLK/1024 */
#define TCCR0B_CS_OVER_1024 (U8_T)( (1u << TCCR0B_CS02) | (1u << TCCR0B_CS00) )
/** Select the external clock on T0 (rising edge) */
#define TCCR0B_CS_EXTERNAL_RISING \
    (U8_T)( (1u << TCCR0B_CS02) | (1u << TCCR0B_CS01) | (1u << TCCR0B_CS00) )
/** Select the external clock on T0 (falling edge) */
#define TCCR0B_CS_EXTERNAL_FALLING \
    (U8_T)( (1u << TCCR0B_CS02) | (1u << TCCR0B_CS01) )

/** Timer/Counter 0 Register */
extern volatile REGISTER_T TCNT0;

/** Timer/Counter 0 Output Compare Register A */
extern volatile REGISTER_T OCR0A;
/** Timer/Counter 0 Output Compare Register B */
extern volatile REGISTER_T OCR0B;


/** Timer/Counter Interrupt Mask Register */
typedef union {
    struct {
        /** Timer/Counter 0 Overflow Interrupt Enable */
        VBOOL_T TOIE0       : 1;
        /** Timer/Counter 0 Output Compare Match A Interrupt Enable */
        VBOOL_T OCIE0A      : 1;
        /** Timer/Counter 0 Output Compare Match B Interrupt Enable */
        VBOOL_T OCIE0B      : 1;
        VBOOL_T Reserved3   : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        VBOOL_T Reserved6   : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TIMSK0_T;
/** Timer/Counter Interrupt Mask Register */
extern volatile TIMSK0_T TIMSK0;


/** Timer/Counter 0 Interrupt Flag Register */
typedef union {
    struct {
        /** Timer/Counter 0 Overflow Flag */
        VBOOL_T TOV0        : 1;
        /** Timer/Counter 0 Output Compare A Match Flag */
        VBOOL_T OCF0A       : 1;
        /** Timer/Counter 0 Output Compare B Match Flag */
        VBOOL_T OCF0B       : 1;
        VBOOL_T Reserved3   : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        VBOOL_T Reserved6   : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TIFR0_T;
/** Timer/Counter 0 Interrupt Flag Register */
extern volatile TIFR0_T TIFR0;


/** Timer/Counter 1 Control Register A */
typedef union {
    struct {
        VBOOL_T WGM10       : 1;
        VBOOL_T WGM11       : 1;
        VBOOL_T Reserved2   : 1;
        VBOOL_T Reserved3   : 1;
        /** Compare Output Mode for Channel B */
        VBOOL_T COM1B0      : 1;
        VBOOL_T COM1B1      : 1;
        /** Compare Output Mode for Channel A */
        VBOOL_T COM1A0      : 1;
        VBOOL_T COM1A1      : 1;
    } bits;
    VU8_T byte;
} TCCR1A_T;
/** Timer/Counter 1 Control Register A */
extern volatile TCCR1A_T TCCR1A;

/** Position of the WGM10 bit in TCCR1A. */
#define TCCR1A_WGM10 0
/** Position of the WGM11 bit in TCCR1A. */
#define TCCR1A_WGM11 1

/** Configure the Timer/Counter 1 in Normal mode (no output pins). */
#define TCCR1A_MODE_NORMAL (U8_T) 0u
/** Enable Fast PWM mode, cycling on comparison match with OCR1A. */
#define TCCR1A_MODE_FAST_PWM_OCR1A (U8_T)( (1u << TCCR1A_WGM10) | (1u << TCCR1A_WGM11) )
/** Enable Phase Correct PWM mode, cycling on comparison match with OCRC1A. */
#define TCCR1A_MODE_PHASE_CORRECT_PWM_OCR1A (U8_T)( (1u << TCCR1A_WGM11) | (1u << TCCR1A_WGM10) )

/** Timer/Counter 1 Control Register B */
typedef union {
    struct {
        /** Clock Select */
        VBOOL_T CS10        : 1;
        VBOOL_T CS11        : 1;
        VBOOL_T CS12        : 1;
        /** Waveform Generation Mode */
        VBOOL_T WGM12       : 1;
        VBOOL_T WGM13       : 1;
        VBOOL_T Reserved5   : 1;
        /** Input Capture Edge Select */
        VBOOL_T ICES1       : 1;
        /** Input Capture Noise Canceler */
        VBOOL_T ICNC1       : 1;
    } bits;
    VU8_T byte;
} TCCR1B_T;
/** Timer/Counter 1 Control Register B */
extern volatile TCCR1B_T TCCR1B;

/** Bit position of WGM12 in TCCR1B. */
#define TCCR1B_WGM12 3
/** Bit position of WGM13 in TCCR1B. */
#define TCCR1B_WGM13 4

#define TCCR1B_MODE_CLEAR_ON_MATCH_A (U8_T)( 1u << TCCR1B_WGM12 )
/** Enable Fast PWM mode, cycling on comparison match with OCR1A. */
#define TCCR1B_MODE_FAST_PWM_OCR1A (U8_T)( (1u << TCCR1B_WGM13) | (1u << TCCR1B_WGM12) )
/** Enable Phase Correct PWM mode, cycling on comparison match with OCR1A. */
#define TCCR1B_MODE_PHASE_CORRECT_PWM_OCR1A (U8_T)( 1u << TCCR1B_WGM13 )

/** Clock select bit 0 */
#define TCCR1B_CS10 0
/** Clock select bit 1 */
#define TCCR1B_CS11 1
/** Clock select bit 2 */
#define TCCR1B_CS12 2
/** Select the bits for the Clock Select in TCCR1B */
#define TCCR1B_CS_MASK \
    (U8_T)( (1u << TCCR1B_CS12) | (1u << TCCR1B_CS11) | (1u << TCCR1B_CS10) )
/** Set the Timer/Counter 1 Prescaler to fclk/256 */
#define TCCR1B_CS_OFF (U8_T) 0
/** Set the Timer/Counter 1 Prescaler to fclk/256 */
#define TCCR1B_CS_OVER_1 (U8_T) ( (1u << TCCR1B_CS10) )
/** Set the Timer/Counter 1 Prescaler to fclk/256 */
#define TCCR1B_CS_OVER_8 (U8_T) ( (1u << TCCR1B_CS11) )
/** Set the Timer/Counter 1 Prescaler to fclk/256 */
#define TCCR1B_CS_OVER_64 (U8_T) ( (1u << TCCR1B_CS11) | (1u << TCCR1B_CS10) )
/** Set the Timer/Counter 1 Prescaler to fclk/256 */
#define TCCR1B_CS_OVER_256 (U8_T) ( (1u << TCCR1B_CS12) )
/** Set the Timer/Counter 1 Prescaler to fclk/256 */
#define TCCR1B_CS_OVER_1024 (U8_T) ( (1u << TCCR1B_CS12) | (1u << TCCR1B_CS10) )
/** Select the external clock on T1 (rising edge) */
#define TCCR1B_CS_EXTERNAL_RISING \
    (U8_T)( (1u << TCCR1B_CS12) | (1u << TCCR1B_CS11) | (1u << TCCR1B_CS10) )
/** Select the external clock on T1 (falling edge) */
#define TCCR1B_CS_EXTERNAL_FALLING \
    (U8_T)( (1u << TCCR1B_CS12) | (1u << TCCR1B_CS11) )


/** Timer/Counter 1 */
extern volatile HALFWORD_T TCNT1;

/** Output Compare Register 1 A */
extern volatile HALFWORD_T OCR1A;

/** Output Compare Register 1 B */
extern volatile HALFWORD_T OCR1B;


/** Timer/Counter 1 Interrupt Mask Register */
typedef union {
    struct {
        /** Timer/Counter 1, Overflow Interrupt Enable */
        VBOOL_T TOIE1       : 1;
        /** Timer/Counter 1, Output Compare A Match Interrupt Enable */
        VBOOL_T OCIE1A      : 1;
        /** Timer/Counter 1, Output Compare B Match Interrupt Enable */
        VBOOL_T OCIE1B      : 1;
        VBOOL_T Reserved3   : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        VBOOL_T Reserved6   : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TIMSK1_T;
/** Timer/Counter 1 Interrupt Mask Register */
extern volatile TIMSK1_T TIMSK1;

/** Enable interrupts for Timer/Counter 1 */
#define TIMSK_ENABLE_INTERRUPT TRUE
/** Disable interrupts for Timer/Counter 1 */
#define TIMSK_DISABLE_INTERRUPT FALSE


/** Timer/Counter 1 Interrupt Flag Register */
typedef union {
    struct {
        /** Timer/Counter 1, Overflow Flag */
        VBOOL_T TOV1        : 1;
        /** Timer/Counter 1, Output Compare A Match Flag */
        VBOOL_T OCF1A       : 1;
        /** Timer/Counter 1, Output Compare B Match Flag */
        VBOOL_T OCF1B       : 1;
        VBOOL_T Reserved3   : 1;
        VBOOL_T Reserved4   : 1;
        VBOOL_T Reserved5   : 1;
        VBOOL_T Reserved6   : 1;
        VBOOL_T Reserved7   : 1;
    } bits;
    VU8_T byte;
} TIFR1_T;
/** Timer/Counter 1 Interrupt Flag Register */
extern volatile TIFR1_T TIFR1;


#endif