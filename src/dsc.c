#include "dsc.h"


void dsc_init(void)
{
    DDR_CANBOARD.bits.LED1 = DDR_OUTPUT;
    DDR_CANBOARD.bits.LED2 = DDR_OUTPUT;
}


void dsc_led_toggle(DSC_CANBOARD_LED_T led)
{
    if (DSC_CANBOARD_LED_1 == led) {
        PORT_CANBOARD.bits.LED1 = !PORT_CANBOARD.bits.LED1;
    } else if (DSC_CANBOARD_LED_2 == led) {
        PORT_CANBOARD.bits.LED2 = !PORT_CANBOARD.bits.LED2;
    }
}