#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

/* 16-MHz */
#define F_CLK 16000000
/* Prescale = F/CLK / 256 */
#define SCHEDULER_TIMER_PRESCALE_DIVISOR 256
/* Seconds per overflow of the 16-bit timer */
#define SCHEDULER_TIMER_SECONDS_PER_OVERFLOW \
    ( (FLOAT_T) U16_T_MAX / ( (FLOAT_T) F_CLK / (FLOAT_T) SCHEDULER_TIMER_PRESCALE_DIVISOR ) )
     
/* 50-ms */
#define SCHEDULER_TIMER_INTERVAL_SECONDS 0.050f
#define SCHEDULER_TIMER_BASE_VALUE \
    ( \
        (U16_T) ((FLOAT_T) U16_T_MAX \
        - ((SCHEDULER_TIMER_INTERVAL_SECONDS / SCHEDULER_TIMER_SECONDS_PER_OVERFLOW) \
        * (FLOAT_T) U16_T_MAX)) \
    )


typedef enum {
    TASK_EXEC_SPI_TX,
    TASK_EXEC_SPI_RX,
    TASK_BIT_ROM,
    TASK_EMPTY3,
    TASK_EMPTY4,
    TASK_EMPTY5,
    TASK_EMPTY6,
    TASK_EMPTY7,
    TASK_EMPTY8,
    TASK_EMPTY9,
    TASK_EMPTY10,
    TASK_EMPTY11,
    TASK_EMPTY12,
    TASK_EMPTY13,
    TASK_EMPTY14,
    TASK_EMPTY15,
    TASK_EMPTY16,
    TASK_EMPTY17,
    TASK_EMPTY18,
    TASK_EMPTY19,
    TASK_N
} SCHEDULER_TASK_T;

typedef enum {
    SCHEDULER_RUNNING,
    SCHEDULER_FINISHED,
    SCHEDULER_IDLE
} SCHEDULER_STATE_T;

typedef void ((*TASK_FN_T)(void));


void scheduler_init(void);
void scheduler_step(void);

/* Runs when there is no other task to run this major cycle */
void task_empty(void);



#endif