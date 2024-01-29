#ifndef __DSC_H__
#define __DSC_H__

typedef enum {
    DSC_LED_CANBOARD_1,
    DSC_LED_CANBOARD_2,
    DSC_LED_BUILTIN
} DSC_LED_T;


typedef enum {
    DSC_LED_DISABLED,
    DSC_LED_ENABLED
} DSC_LED_MODE_T;

typedef enum {
    DSC_LED_OFF,
    DSC_LED_ON
} DSC_LED_STATE_T;


void dsc_init(void);

void dsc_led_mode(DSC_LED_T led, DSC_LED_MODE_T mode);
void dsc_led_toggle(DSC_LED_T led);
void dsc_led_set(DSC_LED_T led, DSC_LED_STATE_T);

#endif