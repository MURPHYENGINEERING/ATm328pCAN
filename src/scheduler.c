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
#include "cnc.h"
#include "memory.h"


TASK_FN_T p_tasks[SCHEDULER_TASKS_N] = {
    &task_watchdog_strobe,
    &task_cnc_rx,
    &task_can_rx,
    &task_demo,
    &task_can_tx,
    &task_bit,
    &task_fai,
    &task_empty,
    &task_empty,
    &task_empty
};

volatile SIZE_T g_task_idx;
volatile SCHEDULER_STATE_T g_scheduler_state;


/*******************************************************************************
 * Iterate to the next task on a regular interval, or log a fault if the current
 * task is overrunning its minor cycle period.
 * This function resets the Timer/Counter 1 value to ensure a regular period of
 * 50-ms.
 ******************************************************************************/
ISR(TIMER1_COMPA_vect)
{
    if (SCHEDULER_RUNNING == g_scheduler_state) {
        fai_pass_fail_logger(FAI_FAULT_ID_TASK_OVERRUN, FAIL, g_task_idx);

    } else if (SCHEDULER_FINISHED == g_scheduler_state) {
        /* Next task */
        g_task_idx = (SIZE_T)( (g_task_idx + 1u) % SCHEDULER_TASKS_N );
        
        g_scheduler_state = SCHEDULER_IDLE;
    } else {
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
    }
}


/*******************************************************************************
 * Initialize the scheduler, including the scheduler timer, to the default/idle
 * state.
 ******************************************************************************/
void scheduler_init(void)
{
    g_task_idx = 0u;
    g_scheduler_state = SCHEDULER_IDLE;
    
    timer1_init(
        TIMER_MODE_CLEAR_ON_MATCH_A, 
        TIMER1_PRESCALE_OVER_256,
        TIMER1_OUTPUT_PINS_DISABLED,
        TIMER1_COMPA_INTERRUPT_ENABLED
    );
    timer1_set_comp(TIMER1_COMP_A, SCHEDULER_TIMER_BASE_VALUE);
}


/*******************************************************************************
 * Perform the current scheduler task.
 * This function toggles the built-in LED on the CAN board (LED 1) every step.
 ******************************************************************************/
void scheduler_step(void)
{
    if (SCHEDULER_IDLE == g_scheduler_state) {
        dsc_led_toggle(DSC_LED_CANBOARD_2);

        g_scheduler_state = SCHEDULER_RUNNING;
        p_tasks[g_task_idx]();
        g_scheduler_state = SCHEDULER_FINISHED;
    }
}


/*******************************************************************************
 * This task does nothing and exists solely to occupy an unused minor cycle.
 ******************************************************************************/
void task_empty(void)
{
}