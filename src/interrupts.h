#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__


/** Defines an Interrupt Service Routine */
#define ISR(vect) \
    void vect(void) __attribute__((signal, used, externally_visible)); \
    void vect(void)


#ifdef ATM328P
#include "atm328p/atm328p_interrupts.h"
#endif

#endif