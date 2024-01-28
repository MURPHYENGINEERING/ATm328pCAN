#include "types.h"
#include "memory.h"
#include "watchdog.h"
#include "interrupts.h"
#include "timer.h"
#include "dsc.h"
#include "spi.h"
#include "fifo.h"


ISR(TIMER1_OVF_vect)
{
    dsc_led_toggle(DSC_CANBOARD_LED_1);
    dsc_led_toggle(DSC_CANBOARD_LED_2);
    /* Reset timer to 50-ms */
    TCNT1.halfword = (U16_T) 0;
}


ISR(SPI_STC_vect)
{
}


static void scheduler_init(void)
{
    /* Normal timer mode */
    TCCR1A.byte = TCCR1A_NORMAL_MODE;
    /* Enable prescaler (clk / 256) */
    TCCR1B.byte |= TCCR1B_PRESCALE_OVER_256;
    /* Enable timer overflow interrupt */
    TIMSK1.bits.TOIE1 = TOIE1_ENABLE_INTERRUPT;
}


S32_T main(void)
{
    /* Soft-reset if the watchdog doesn't get strobed */
    watchdog_enable();

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
    }
}