#include "types.h"
#include "memory.h"
#include "watchdog.h"
#include "interrupts.h"
#include "timer.h"
#include "dsc.h"
#include "scheduler.h"
#include "spi.h"

S16_T main(void)
{
    /* Soft-reset if the watchdog doesn't get strobed */
    watchdog_enable();

    /* Initialize GPIOs */
    dsc_init();

    /* Start SPI bus */
    spi_init();

    /* Start task timing */
    scheduler_init();

    /* Enable interrupts */
    sei();

    /* Run forever in interrupt-driven mode */
    while(TRUE)
    {
        scheduler_step();
    }
}