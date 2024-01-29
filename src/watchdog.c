#include "watchdog.h"
#include "atm328p/atm328p_watchdog.h"
#include "atm328p/atm328p_mcu.h"


void watchdog_enable(void)
{
    WDTCSR.bits.WDE = WDE_WATCHDOG_ENABLED;
    /* Time out the watchdog in two major cycles */
    WDTCSR.byte |= WDTCSR_PRESCALE_TIMEOUT_IN_2S
}


void watchdog_disable(void)
{
    /* WDE is overridden by WDRF in MCUSR. This means that WDE is always set 
     * when WDRF is set. To clear WDE, WDRF must be cleared first.
     * - ATmega328P Datasheet page 48 */
    MCUSR.bits.WDRF = WDRF_WATCHDOG_RESET_DISABLED;
    WDTCSR.bits.WDE = WDE_WATCHDOG_DISABLED;
}