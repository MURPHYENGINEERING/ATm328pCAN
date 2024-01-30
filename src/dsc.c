#include "dsc.h"
#include "atm328p/atm328p_dsc.h"


void dsc_init(void)
{
    dsc_led_mode(DSC_LED_CANBOARD_1, ENABLED);
    dsc_led_mode(DSC_LED_CANBOARD_2, ENABLED);
    dsc_led_set(DSC_LED_CANBOARD_1, OFF);
    dsc_led_set(DSC_LED_CANBOARD_2, OFF);
}


void dsc_led_mode(DSC_LED_T led, ENABLE_T mode) {
    if (ENABLED == mode) {
        switch (led) {
            case DSC_LED_BUILTIN:
                DDR_BUILTIN.bits.LED = DDR_OUTPUT;
            break;

            case DSC_LED_CANBOARD_1:
                DDR_CANBOARD.bits.LED1 = DDR_OUTPUT;
            break;

            case DSC_LED_CANBOARD_2:
                DDR_CANBOARD.bits.LED2 = DDR_OUTPUT;
            break;

            default:
            /* Software error */
            break;
        }
    } else {
        switch (led) {
            case DSC_LED_BUILTIN:
                DDR_BUILTIN.bits.LED = DDR_INPUT;
            break;

            case DSC_LED_CANBOARD_1:
                DDR_CANBOARD.bits.LED1 = DDR_INPUT;
            break;

            case DSC_LED_CANBOARD_2:
                DDR_CANBOARD.bits.LED2 = DDR_INPUT;
            break;

            default:
            /* Software error */
            break;
        }
    }
}


void dsc_led_toggle(DSC_LED_T led)
{
    switch (led) {
        case DSC_LED_CANBOARD_1:
            PORT_CANBOARD.bits.LED1 = !PORT_CANBOARD.bits.LED1;
            break;
        case DSC_LED_CANBOARD_2:
            PORT_CANBOARD.bits.LED2 = !PORT_CANBOARD.bits.LED2;
            break;
        case DSC_LED_BUILTIN:
            PORT_BUILTIN.bits.LED = !PORT_BUILTIN.bits.LED;
            break;
        default:
            /* Software error */
            break;
    }
}


void dsc_led_set(DSC_LED_T led, ON_T state)
{
    if (OFF == state) {
        switch (led) {
            case DSC_LED_BUILTIN:
                PORT_BUILTIN.bits.LED = OPEN;
            break;

            case DSC_LED_CANBOARD_1:
                PORT_CANBOARD.bits.LED1 = OPEN;
            break;

            case DSC_LED_CANBOARD_2:
                PORT_CANBOARD.bits.LED2 = OPEN;
            break;

            default:
            /* Software error */
            break;
        }
    } else {
        switch (led) {
            case DSC_LED_BUILTIN:
                PORT_BUILTIN.bits.LED = GND;
            break;

            case DSC_LED_CANBOARD_1:
                PORT_CANBOARD.bits.LED1 = GND;
            break;

            case DSC_LED_CANBOARD_2:
                PORT_CANBOARD.bits.LED2 = GND;
            break;

            default:
            /* Software error */
            break;
        }
    }
}