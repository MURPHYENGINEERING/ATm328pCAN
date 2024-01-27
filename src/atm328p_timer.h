#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

/* Timer/Counter 0 Control Register A */
#define TCCR0A *((VU8_T*)0x44u)
/* Timer/Counter 0 Control Register B */
#define TCCR0B *((VU8_T*)0x45u) 
/* Clock Select Bits */
#define CS00 0
#define CS02 2

/* Timer/Counter 0 Register */
#define TCNT0 *((VU8_T*)0x46u)
/* Timer/Counter 0 Output Compare Register A */
#define OCR0A *((VU8_T*)0x47u)
/* Timer/Counter 0 Output Compare Register B */
#define OCR0B *((VU8_T*)0x48u)

/* Timer/Counter Interrupt Mask Register */
#define TIMSK0 *((VU8_T*)0x6Eu)
/* Timer/Counter0 Output Compare Match A Interrupt Enable */
#define OCIE0A 1

/* Timer/Counter 0 Interrupt Flag Register */
#define TIFR0 *((VU8_T*)0x35u)
/* Timer/Counter 0 Overflow Flag */
#define TOV0 0
/* Timer/Counter 0 Output Compare A Match Flag */
#define OCF0A 1
/* Timer/Counter 0 Output Compare B Match Flag */
#define OCF0B 2


/* Timer/Counter 1 Control Register A */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        /* Compare Output Mode for Channel B */
        VBOOL_T COM1B0 : 1;
        VBOOL_T COM1B1 : 1;
        /* Compare Output Mode for Channel A */
        VBOOL_T COM1A0 : 1;
        VBOOL_T COM1A1 : 1;
    } bits;
    VU8_T byte;
} TCCR1A_T;

extern volatile TCCR1A_T TCCR1A;

#define TCCR1A_NORMAL_MODE 0


/* Timer/Counter 1 Control Register B */
typedef union {
    struct {
        /* Clock Select */
        /* 0, 0, 0 = no clock source 
         * 0, 0, 1 = clk/1 
         * 0, 1, 0 = clk/8
         * 0, 1, 1 = clk/64
         * 1, 0, 0 = clk/256
         * 1, 0, 1 = clk/1024 */
        VBOOL_T CS10 : 1;
        VBOOL_T CS11 : 1;
        VBOOL_T CS12 : 1;
        /* Waveform Generation Mode */
        VBOOL_T WGM12 : 1;
        VBOOL_T WGM13 : 1;
        VBOOL_T Reserved5 : 1;
        /* Input Capture Edge Select */
        VBOOL_T ICES1 : 1;
        /* Input Capture Noise Canceler */
        VBOOL_T ICNC1 : 1;
    } bits;
    VU8_T byte;
} TCCR1B_T;

extern volatile TCCR1B_T TCCR1B;


/* Timer/Counter 1 */
extern volatile HALFWORD_T TCNT1;

/* Output Compare Register 1 A */
extern volatile HALFWORD_T OCR1A;

/* Output Compare Register 1 B */
extern volatile HALFWORD_T OCR1B;


/* Timer/Counter 1 Interrupt Mask Register */
typedef union {
    struct {
        /* Timer/Counter 1, Overflow Interrupt Enable */
        VBOOL_T TOIE1  : 1;
        /* Timer/Counter 1, Output Compare A Match Interrupt Enable */
        VBOOL_T OCIE1A : 1;
        /* Timer/Counter 1, Output Compare B Match Interrupt Enable */
        VBOOL_T OCIE1B : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} TIMSK1_T;

extern volatile TIMSK1_T TIMSK1;

#define OCIE1A_ENABLE_INTERRUPT 1


/* Timer/Counter 1 Interrupt Flag Register */
typedef union {
    struct {
        /* Timer/Counter 1, Overflow Flag */
        VBOOL_T TOV1 : 1;
        /* Timer/Counter 1, Output Compare A Match Flag */
        VBOOL_T OCF1A : 1;
        /* Timer/Counter 1, Output Compare B Match Flag */
        VBOOL_T OCF1B : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} TIFR1_T;

extern volatile TIFR1_T TIFR1;


#endif