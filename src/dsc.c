#include "dsc.h"


/*******************************************************************************
 *
 ******************************************************************************/
void dsc_init(void)
{
    dsc_led_mode(DSC_LED_CANBOARD_1, ENABLED);
    dsc_led_mode(DSC_LED_CANBOARD_2, ENABLED);
    dsc_led_set(DSC_LED_CANBOARD_1, OFF);
    dsc_led_set(DSC_LED_CANBOARD_2, OFF);
}