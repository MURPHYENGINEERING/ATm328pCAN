#ifndef __DSC_H__
#define __DSC_H__

#include "types.h"


/** Represents one of the built-in LEDs on the UNO and MCP2515 development boards. */
typedef enum {
    /** LED 1 on the MCP2515 development board. */
    DSC_LED_CANBOARD_1,
    /** LED 2 on the MCP2515 development board. */
    DSC_LED_CANBOARD_2,
    /** LED 1 on the Arduino UNO development board. */
    DSC_LED_BUILTIN
} DSC_LED_T;


void dsc_init(void);

void dsc_led_mode(DSC_LED_T led, ENABLE_T mode);
void dsc_led_toggle(DSC_LED_T led);
void dsc_led_set(DSC_LED_T led, ON_T);

#endif