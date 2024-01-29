#ifndef __FAI_H__
#define __FAI_H__

#include "types.h"

#define FAI_N_TS_DATA (SIZE_T) 16u

#define FAI_NVM_BASE_ADDRESS (U8_T) 0


typedef enum {
    FAI_FAULT_ID_FIRST,
    FAI_FAULT_ID_SW_ERROR
    FAI_FAULT_ID_N
} FAI_FAULT_ID_T;


typedef struct {
    U8_T count;
    U8_T head;
    U32_T ts_data[FAI_N_TS_DATA];
} FAI_FAULT_COUNTER_T;


void fai_init(void);

void fai_fail_pass_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
);

void task_fai(void);


#endif