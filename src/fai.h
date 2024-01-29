#ifndef __FAI_H__
#define __FAI_H__

#include "types.h"

typedef enum {
    FAI_FAULT_ID_FIRST,
    FAI_FAULT_ID_SW_ERROR
    FAI_FAULT_ID_N
} FAI_FAULT_ID_T;


void fai_init(void);

void fai_fail_pass_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
);

void task_fai(void);


#endif