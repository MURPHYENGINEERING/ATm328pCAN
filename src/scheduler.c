#include "scheduler.h"
#include "interrupts.h"
#include "timer.h"
#include "dsc.h"
#include "spi.h"
#include "bit.h"


TASK_FN_T tasks[(SIZE_T) TASK_N] = {
    &task_exec_spi_tx,      /* TASK_EXEC_SPI_TX */
    &task_exec_spi_rx,      /* TASK_EXEC_SPI_RX */
    &task_bit_rom,          /* TASK_EXEC_BIT_ROM */
    &task_empty,            /* TASK_EMPTY3 */
    &task_empty,            /* TASK_EMPTY4 */
    &task_empty,            /* TASK_EMPTY5 */
    &task_empty,            /* TASK_EMPTY6 */
    &task_empty,            /* TASK_EMPTY7 */
    &task_empty,            /* TASK_EMPTY8 */
    &task_empty,            /* TASK_EMPTY9 */
    &task_empty,            /* TASK_EMPTY10 */
    &task_empty,            /* TASK_EMPTY11 */
    &task_empty,            /* TASK_EMPTY12 */
    &task_empty,            /* TASK_EMPTY13 */
    &task_empty,            /* TASK_EMPTY14 */
    &task_empty,            /* TASK_EMPTY15 */
    &task_empty,            /* TASK_EMPTY16 */
    &task_empty,            /* TASK_EMPTY17 */
    &task_empty,            /* TASK_EMPTY18 */
    &task_empty             /* TASK_EMPTY19 */
};

SCHEDULER_TASK_T g_task;


ISR(TIMER1_OVF_vect)
{
    cli();

    tasks[(SIZE_T) g_task]();

    g_task = (SCHEDULER_TASK_T)( (SIZE_T) g_task + 1 );
    if (TASK_N == g_task) {
        g_task = (SCHEDULER_TASK_T) 0;
    }

    /* Reset timer to 50-ms */
    timer1_set(SCHEDULER_TIMER_BASE_VALUE);
    
    sei();
}


void scheduler_init(void)
{
    timer1_enable(TIMER1_PRESCALE_OVER_256, TIMER1_INTERRUPT_ENABLED);
    timer1_set(SCHEDULER_TIMER_BASE_VALUE);

    g_task = (SCHEDULER_TASK_T) 0;
}


void task_empty(void)
{
}