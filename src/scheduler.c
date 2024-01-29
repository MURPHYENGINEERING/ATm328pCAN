#include "scheduler.h"
#include "interrupts.h"
#include "timer.h"
#include "dsc.h"
#include "spi.h"
#include "bit.h"
#include "can.h"
#include "demo.h"


TASK_FN_T tasks[(SIZE_T) N_TASKS] = {
    &task_demo_tx,          /* TASK_EXEC_DEMO_TX */
    &task_can_tx,           /* TASK_EXEC_CAN_TX */
    &task_can_rx,           /* TASK_EXEC_CAN_RX */
    &task_demo_rx,          /* TASK_EXEC_DEMO_RX */
    &task_bit_rom,          /* TASK_EXEC_BIT_ROM */
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

volatile SIZE_T g_task_idx;
volatile SCHEDULER_STATE_T g_scheduler_state;


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


void scheduler_init(void)
{
    timer1_enable(TIMER1_PRESCALE_OVER_256, TIMER1_INTERRUPT_ENABLED);
    timer1_set(SCHEDULER_TIMER_BASE_VALUE);

    g_task_idx = 0;
    g_scheduler_state = SCHEDULER_IDLE;
}


void scheduler_step(void)
{
    if (SCHEDULER_IDLE == g_scheduler_state) {
        dsc_led_toggle(DSC_LED_CANBOARD_1);

        g_scheduler_state = SCHEDULER_RUNNING;
        tasks[g_task_idx]();
        g_scheduler_state = SCHEDULER_FINISHED;
    }
}


void task_empty(void)
{
}