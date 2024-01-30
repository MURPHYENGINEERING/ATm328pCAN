#ifndef __DSC_H__
#define __DSC_H__

#include "types.h"

typedef enum {
    DSC_LED_CANBOARD_1,
    DSC_LED_CANBOARD_2,
    DSC_LED_BUILTIN
} DSC_LED_T;


void dsc_init(void);

void dsc_led_mode(DSC_LED_T led, ENABLE_T mode);
void dsc_led_toggle(DSC_LED_T led);
void dsc_led_set(DSC_LED_T led, ON_T);

#endif