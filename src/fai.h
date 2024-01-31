#ifndef __FAI_H__
#define __FAI_H__

#include "types.h"

#define FAI_TS_DATA_LEN (SIZE_T) 16u

#define FAI_NVM_BASE_ADDRESS (SIZE_T) 0


typedef enum {
    FAI_FAULT_ID_SW_ERROR,
    FAI_FAULT_ID_BAD_FAULT,
    FAI_FAULT_ID_BUFFER_OVERFLOW,
    FAI_FAULT_ID_USART_RX_BUFFER_OVERFLOW,
    FAI_FAULT_ID_USART_PARITY_ERROR,
    FAI_FAULT_ID_N
} FAI_FAULT_ID_T;


typedef struct {
    U8_T count;
    U8_T head;
    U32_T ts_data[FAI_TS_DATA_N];
} FAI_FAULT_COUNTER_T;


void fai_init(void);

void fai_pass_fail_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
);

FAI_FAULT_COUNTER_T fai_fault_reporter(FAI_FAULT_ID_T fault_id);

void task_fai(void);


#endif