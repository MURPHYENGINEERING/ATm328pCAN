#ifndef __ATM328P_INTERRUPTS_H__
#define __ATM328P_INTERRUPTS_H__

#define ISR(vect) \
    void vect(void) __attribute__((signal, used, externally_visible)); \
    void vect(void)

/* Timer/Counter 1 Compare Match A */
#define TIMER1_COMPA_vect   __vector_11
/* Timer/Counter 1 Compare Match B */
#define TIMER1_COMPB_vect   __vector_12
/* Timer/Counter 1 Overflow */
#define TIMER1_OVF_vect     __vector_13

/* Timer/Counter 0 Compare Match A */
#define TIMER0_COMPA_vect   __vector_14
/* Timer/Counter 0 Compare Match B */
#define TIMER0_COMPB_vect   __vector_15
/* Timer/Counter 0 Overflow */
#define TIMER0_OVF_vect     __vector_16

/* SPI Serial Transfer Complete */
#define SPI_STC_vect        __vector_17

/* USART Interrupts */
#define USART0_RX_vect       __vector_18
#define USART0_UDRE_vect     __vector_19
#define USART0_TX_vect       __vector_20


#define sei() __asm volatile( "sei" ::: "memory" )
#define cli() __asm volatile( "cli" ::: "memory" )

#endif