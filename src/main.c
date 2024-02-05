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
    /* Enable interrupts */
    sei();

    /* Soft-reset if the watchdog doesn't get strobed in 4 major cycles */
    watchdog_enable();

    /* Initialize fault logging */
    fai_init();

    /* Initialize GPIOs */
    dsc_init();

    SPI_CONFIG_T spi_config;
    spi_config.enable = ENABLED;
    spi_config.mode = SPI_MODE_MASTER;
    spi_config.endian = SPI_ENDIAN_MSB_FIRST;
    spi_config.phase = SPI_PHASE_SAMPLE_ON_LEADING;
    spi_config.polarity = SPI_POLARITY_LEADING_IS_RISING;
    spi_config.prescale = SPI_PRESCALE_OVER_256;
    spi_config.interrupts = ENABLED;

    /* Start SPI bus */
    spi_init(spi_config);

    /* Start CAN bus */
    can_init();

    USART_CONFIG_T usart_config;
    usart_config.mode = USART_MODE_ASYNCHRONOUS;
    usart_config.baud = USART_BAUD_9600;
    usart_config.character_size = USART_CHARACTER_SIZE_8;
    usart_config.stop_bits = USART_STOP_BITS_1;
    usart_config.parity = USART_PARITY_MODE_EVEN;

    /* Start USART bus */
    usart_init(usart_config);

    /* Initiate conversion on sample and wait for it to finish before returning. */
    adc_init(ADC_MODE_BLOCKING);

    /* Command and Control */
    cnc_init();

    /* Demo application */
    demo_init();

    /* Start task timing */
    scheduler_init();

    /* Run tasks forever. Tasks are switched by the scheduler on a 50-ms
     * interval. */
    while(TRUE)
    {
        scheduler_step();
    }
}