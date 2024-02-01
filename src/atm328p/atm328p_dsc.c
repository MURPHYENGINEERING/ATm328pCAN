#include "atm328p_dsc.h"
#include "dsc.h"


/*******************************************************************************
 * Change the pin mode of the given LED pin.
 * \param[in] led   The given LED to be configured.
 * \param[in] mode  `ENABLED` to enable output mode and `DISABLED` to configure
 *                  the pin as an input.
 ******************************************************************************/
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


/*******************************************************************************
 * Toggle the state (on or off) of the given LED.
 * \param[in] led   The given LED whose state will be toggled.
 ******************************************************************************/
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


/*******************************************************************************
 * Set the state of the given LED pin to the given state.
 * \param[in] led   The given LED whose state will be set.
 * \param[in] state The state (`ON` or `OFF`) to be set.
 ******************************************************************************/
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