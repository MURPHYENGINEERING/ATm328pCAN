#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

typedef enum {
    TIMER1_PRESCALE_OVER_256
} TIMER1_PRESCALE_T;


typedef enum {
    TIMER1_INTERRUPT_DISABLED,
    TIMER1_INTERRUPT_ENABLED
} TIMER1_INTERRUPT_MODE_T;


void timer1_enable(
    TIMER1_PRESCALE_T prescale, 
    TIMER1_INTERRUPT_MODE_T interrupt_mode
);

void timer1_set(U16_T value);


#endif