#ifndef __DSC_H__
#define __DSC_H__

#include "atm328p/atm328p_dsc.h"

typedef enum {
    DSC_CANBOARD_LED_1,
    DSC_CANBOARD_LED_2
} DSC_CANBOARD_LED_T;


void dsc_init(void);

void dsc_led_toggle(DSC_CANBOARD_LED_T led);

#endif