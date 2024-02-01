#include "scheduler.h"
#include "interrupts.h"
#include "timer.h"
#include "dsc.h"
#include "spi.h"
#include "bit.h"
#include "can.h"
#include "fai.h"
#include "watchdog.h"
#include "demo.h"


TASK_FN_T tasks[N_TASKS] = {
    &task_watchdog_strobe,
    &task_demo_tx,
    &task_can_tx,
    &task_can_rx,
    &task_demo_rx,
    &task_bit_rom,
    &task_fai,
    &task_empty,
    &task_empty,
    &task_empty
};

volatile SIZE_T g_task_idx;
volatile SCHEDULER_STATE_T g_scheduler_state;


/*******************************************************************************
 *
 ******************************************************************************/
ISR(TIMER1_OVF_vect)
{
    if (SCHEDULER_RUNNING == g_scheduler_state) {
        /* Task overrun fault */

    } else if (SCHEDULER_FINISHED == g_scheduler_state) {
        /* Next task */
        ++g_task_idx;
        if (N_TASKS == g_task_idx) {
            g_task_idx = 0;
        }
        g_scheduler_state = SCHEDULER_IDLE;
    } else {
        /* Software error */
    }

    /* Reset timer to 50-ms */
    timer1_set(SCHEDULER_TIMER_BASE_VALUE);
}


/*******************************************************************************
 *
 ******************************************************************************/
void scheduler_init(void)
{
    g_task_idx = 0;
    g_scheduler_state = SCHEDULER_IDLE;
    
    timer1_enable(TIMER1_PRESCALE_OVER_256, TIMER1_INTERRUPT_ENABLED);
    timer1_set(SCHEDULER_TIMER_BASE_VALUE);
}


/*******************************************************************************
 *
 ******************************************************************************/
void scheduler_step(void)
{
    if (SCHEDULER_IDLE == g_scheduler_state) {
        dsc_led_toggle(DSC_LED_CANBOARD_1);

        g_scheduler_state = SCHEDULER_RUNNING;
        tasks[g_task_idx]();
        g_scheduler_state = SCHEDULER_FINISHED;
    }
}


/*******************************************************************************
 *
 ******************************************************************************/
void task_empty(void)
{
}