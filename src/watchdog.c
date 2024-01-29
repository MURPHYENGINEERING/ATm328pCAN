#include "watchdog.h"


void watchdog_enable(void)
{
    WDTCSR.bits.WDE = WDE_WATCHDOG_ENABLED;
}


void watchdog_disable(void)
{
    WDTCSR.bits.WDE = WDE_WATCHDOG_DISABLED;
}