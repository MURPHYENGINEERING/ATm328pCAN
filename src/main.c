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
#include "usart.h"
#include "adc.h"
#include "cnc.h"
#include "demo.h"


/*******************************************************************************
 * Start the application, enabling all hardware and software devices and starting
 * the task scheduler.
 * This function enters an infinite loop, executing scheduled tasks until forever.
 ******************************************************************************/
S16_T main(void)
{
    /* Disable interrupts until everything is ready */
    cli();

    /* Soft-reset if the watchdog doesn't get strobed in 4 major cycles */
    watchdog_enable();

    /* Initialize fault logging */
    fai_init();

    /* Initialize GPIOs */
    dsc_init();

    /* Start SPI bus */
    spi_init(can_get_spi_config());

    /* Start CAN bus */
    can_init();

    /* Start USART bus */
    usart_init(cnc_get_usart_config());

    /* Initiate conversion on sample and wait for it to finish before returning. */
    adc_init(ADC_MODE_BLOCKING);

    /* Command and Control */
    cnc_init();

    /* Demo application */
    demo_init();

    /* Start task timing */
    scheduler_init();

    /* Enable interrupts to allow the scheduler timer and peripherals to run. */
    sei();

    /* Run tasks forever. Tasks are switched by the scheduler on a 50-ms
     * interval. */
    while(TRUE)
    {
        scheduler_step();
    }
}