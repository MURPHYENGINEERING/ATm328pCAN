#include "fai.h"
#include "eeprom.h"

#define FAI_MAX_FAULTS U16_T_MAX

U16_T g_fault_counters[(SIZE_T) FAI_FAULT_ID_N];
BOOL_T g_pending_faults;


void fai_init(void)
{
    SIZE_T i;

    for (i = 0; (SIZE_T) FAI_FAULT_ID_N > i; ++i) {
        g_fault_counters[i] = 0;
    }

    g_pending_faults = FALSE;
}


void fai_fail_pass_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
)
{
    if ( (FAI_FAULT_ID_FIRST < fault_id) && (FAI_FAULT_ID_N > fault_id) ) {
        /* Update the pending faults table */
        if (FAI_MAX_FAULTS > g_fault_counters[fault_id]) {
            ++g_fault_counters[fault_id];
            g_pending_faults = TRUE;
        }
    } else {
        /* Invalid fault ID */
    }
}


void task_fai(void)
{
    if (TRUE == g_pending_faults) {
        
        g_pending_faults = FALSE;
    }
}