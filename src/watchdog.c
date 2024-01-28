#include "watchdog.h"


void watchdog_enable(void)
{
    WDTCSR.byte = WDTCSR_WATCHDOG_ENABLED;
}


void watchdog_disable(void)
{
    WDTCSR.byte = WDTCSR_WATCHDOG_DISABLED;
}