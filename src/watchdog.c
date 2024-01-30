#include "watchdog.h"
#include "atm328p/atm328p_watchdog.h"
#include "atm328p/atm328p_mcu.h"
#include "atm328p/atm328p_interrupts.h"


void watchdog_enable(void)
{
    cli();
    watchdog_reset();
    /* Time out the watchdog in two major cycles */
    /* For some reason it's very sensitive to the order that these are set.
     * You absolutely must set ENABLE_WATCHDOG and CHANGE_WATCHDOG first,
     * then must set ENABLE_WATCHDOG and the prescaler at the same time. */
    WDTCSR.byte = WDTCSR_ENABLE_WATCHDOG | WDTCSR_CHANGE_WATCHDOG;
    WDTCSR.byte = WDTCSR_ENABLE_WATCHDOG | WDTCSR_PRESCALE_TIMEOUT_IN_2S;
    sei();
}


void watchdog_disable(void)
{
    cli();
    /* WDE is overridden by WDRF in MCUSR. This means that WDE is always set 
     * when WDRF is set. To clear WDE, WDRF must be cleared first.
     * - ATmega328P Datasheet page 48 */
    MCUSR.bits.WDRF = WDRF_WATCHDOG_RESET_DISABLED;
    WDTCSR.bits.WDCE = WDCE_CHANGE_WATCHDOG;
    WDTCSR.bits.WDE = WDE_WATCHDOG_DISABLED;
    sei();
}


void task_watchdog_strobe(void)
{
    watchdog_reset();
}