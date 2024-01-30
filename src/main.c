#include "types.h"
#include "memory.h"
#include "watchdog.h"
#include "interrupts.h"
#include "timer.h"
#include "dsc.h"
#include "scheduler.h"
#include "spi.h"
#include "can.h"
#include "fai.h"


S16_T main(void)
{
    /* Enable interrupts */
    sei();

    /* Soft-reset if the watchdog doesn't get strobed in 2 major cycles */
    watchdog_enable();

    /* Initialize fault logging */
    fai_init();

    /* Initialize GPIOs */
    dsc_init();

    /* Start SPI bus */
    spi_init();

    /* Start CAN bus */
    can_init();

    /* Start task timing */
    scheduler_init();

    /* Run tasks forever. Tasks are switched by the scheduler on a 50-ms
     * interval. */
    while(TRUE)
    {
        scheduler_step();
    }
}