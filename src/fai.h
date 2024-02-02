#ifndef __FAI_H__
#define __FAI_H__

#include "types.h"

/** Number of elements in the `FAI_FAULT_COUNTER` `ts_data` array. */
#define FAI_TS_DATA_LEN (SIZE_T) 4u

/** Address of the "faults have been initialized" flag in NVM. */
#define FAI_NVM_INIT_FLAG_ADDRESS (SIZE_T) 0

/** Base address in the NVM where fault data is stored. */
#define FAI_NVM_BASE_ADDRESS (SIZE_T) 1u

/** Represents a particular fault for logging and reporting. */
typedef enum {
    /** Software error, i.e., a bug. */
    FAI_FAULT_ID_SW_ERROR,
    /** Invalid fault ID passed to logging or reporting */
    FAI_FAULT_ID_BAD_FAULT,
    /** A buffer was full when a write was attempted. */
    FAI_FAULT_ID_BUFFER_OVERFLOW,
    /** The CAN TX buffer was full when a new CAN message was added. */
    FAI_FAULT_ID_CAN_TX_BUFFER_OVERFLOW,
    /** The USART RX buffer was full when data was written to it. */
    FAI_FAULT_ID_USART_RX_BUFFER_OVERFLOW,
    /** The incoming USART byte failed the parity test. */
    FAI_FAULT_ID_USART_PARITY_ERROR,
    /** A minor cycle task was still running when the next minor cycle began. */
    FAI_FAULT_ID_TASK_OVERRUN,
    /** The Command and Control RX queue was full and a command message was lost. */
    FAI_FAULT_ID_CNC_RX_Q_OVERFLOW,
    /** The Command and Control "send CAN message" command was malformed. */
    FAI_FAULT_ID_CNC_SENDCAN_MALFORMED,
    /** Number of fault IDs. */
    FAI_FAULT_ID_N
} FAI_FAULT_ID_T;

/** The first fault ID in the list `FAI_FAULT_ID_T` */
#define FAI_FAULT_ID_FIRST FAI_FAULT_ID_SW_ERROR


/** Stores fault data including number of fault occurrences and troubleshooting
 * data associated with each fault. */
typedef struct {
    /** Number of occurrences of this fault that have been logged. */
    U8_T count;
    /** Write pointer into the `ts_data` array. */
    U8_T head;
    /** Troubleshooting data associated with each logged occurrence. */
    U32_T ts_data[FAI_TS_DATA_LEN];
} FAI_FAULT_COUNTER_T;


void fai_init(void);

void fai_pass_fail_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
);

FAI_FAULT_COUNTER_T fai_fault_reporter(FAI_FAULT_ID_T fault_id);

void fai_read_faults_from_nvm(void);

void fai_clear_faults(void);

void task_fai(void);


#endif