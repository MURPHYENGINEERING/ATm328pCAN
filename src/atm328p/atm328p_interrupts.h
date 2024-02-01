#ifndef __ATM328P_INTERRUPTS_H__
#define __ATM328P_INTERRUPTS_H__

/** Defines an Interrupt Service Routine */
#define ISR(vect) \
    void vect(void) __attribute__((signal, used, externally_visible)); \
    void vect(void)

/** Timer/Counter 1 Compare Match A Interrupt */
#define TIMER1_COMPA_vect   __vector_11
/** Timer/Counter 1 Compare Match B Interrupt */
#define TIMER1_COMPB_vect   __vector_12
/** Timer/Counter 1 Overflow Interrupt */
#define TIMER1_OVF_vect     __vector_13

/** Timer/Counter 0 Compare Match A Interrupt */
#define TIMER0_COMPA_vect   __vector_14
/** Timer/Counter 0 Compare Match B Interrupt */
#define TIMER0_COMPB_vect   __vector_15
/** Timer/Counter 0 Overflow Interrupt */
#define TIMER0_OVF_vect     __vector_16

/** SPI Serial Transfer Complete Interrupt */
#define SPI_STC_vect        __vector_17

/** USART Receive Complete Interrupt */
#define USART0_RX_vect       __vector_18
/** USART Data Register Empty Interrupt */
#define USART0_UDRE_vect     __vector_19
/** USART Transmit Complete Interrupt */
#define USART0_TX_vect       __vector_20

/** ADC Conversion Complete Interrupt */
#define ADC_vect             __vector_21


/** Globally enable interrupts */
#define sei() __asm volatile( "sei" ::: "memory" )
/** Globally disable interrupts */
#define cli() __asm volatile( "cli" ::: "memory" )

#endif