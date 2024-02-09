#include "types.h"
#include "watchdog.h"
#include "interrupts.h"
#include "fai.h"
#include "dsc.h"
#include "can.h"
#include "cnc.h"
#include "demo.h"
#include "scheduler.h"
#include "strap.h"


/*******************************************************************************
 * Start the application, enabling all hardware and software devices and starting
 * the task scheduler.
 * This function enters an infinite loop, executing scheduled tasks until forever.
 ******************************************************************************/
S16_T main(void)
{
    /* Disable interrupts until everything is ready */
    cli();

    /* Initialize fault logging */
    fai_init();

    strap_init();

    /* Initialize GPIOs */
    dsc_init();

    /* Command and Control */
    cnc_init();

    /* Demo application */
    demo_init();

    /* Start task timing */
    scheduler_init();

    /* Soft-reset if the watchdog doesn't get strobed in 4 major cycles */
    watchdog_enable();

    /* Enable interrupts to allow the scheduler timer and peripherals to run. */
    sei();

    /* Run tasks forever. Tasks are switched by the scheduler on a 50-ms
     * interval. */
    while(TRUE)
    {
        scheduler_step();
    }
}