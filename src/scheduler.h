#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

/** The base clock of the ATm328P is 16-MHz. */
#define F_CLK 16000000
/** Timer Prescale = `F_CLK` / 256 */
#define SCHEDULER_TIMER_PRESCALE_DIVISOR 256
/** Seconds per overflow of the 16-bit Timer/Counter 1. */
#define SCHEDULER_TIMER_SECONDS_PER_OVERFLOW \
    ( (FLOAT_T) U16_T_MAX / ( (FLOAT_T) F_CLK / (FLOAT_T) SCHEDULER_TIMER_PRESCALE_DIVISOR ) )
     
/** The Timer/Counter 1 overflows in 50-ms, representing one minor cycle. */
#define SCHEDULER_TIMER_INTERVAL_SECONDS 0.050f
/** Value to which the Timer/Counter 1 should be set to have it overflow in 
  * 50-ms. */
#define SCHEDULER_TIMER_BASE_VALUE \
    ( \
        (U16_T) ((FLOAT_T) U16_T_MAX \
        - ((SCHEDULER_TIMER_INTERVAL_SECONDS / SCHEDULER_TIMER_SECONDS_PER_OVERFLOW) \
        * (FLOAT_T) U16_T_MAX)) \
    )


/** Number of tasks run by the scheduler in round-robin fashion, yielding a 
  * 500-ms major cycle. */
#define N_TASKS 10

/** Represents the current state of the scheduler. */
typedef enum {
    /** A scheduler task is currently running. */
    SCHEDULER_RUNNING,
    /** The previously running scheduler task has completed, but its minor cycle
      * has not yet completed. */
    SCHEDULER_FINISHED,
    /** The previous minor cycle has completed and the next has not yet begun. */
    SCHEDULER_IDLE
} SCHEDULER_STATE_T;


/** Prototype of a scheduler task function. */
typedef void ((*TASK_FN_T)(void));


void scheduler_init(void);
void scheduler_step(void);

void task_empty(void);



#endif