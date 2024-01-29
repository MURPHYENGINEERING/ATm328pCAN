#include "fai.h"
#include "eeprom.h"

#define FAI_MAX_FAULTS U16_T_MAX

FAI_FAULT_COUNTER_T g_fault_counters[(SIZE_T) FAI_FAULT_ID_N];
BOOL_T g_pending_faults;


static void fai_write_faults_to_nvm(void);


void fai_init(void)
{
    SIZE_T i;

    for (i = 0; (SIZE_T) FAI_FAULT_ID_N > i; ++i) {
        g_fault_counters[i].count = 0;
        g_fault_counters[i].head = 0;
        g_fault_counters[i].ts_data = 0;
    }

    g_pending_faults = FALSE;
}


void fai_fail_pass_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
)
{
    FAI_FAULT_COUNTER_T* p_fault;

    if ( (FAI_FAULT_ID_FIRST < fault_id) && (FAI_FAULT_ID_N > fault_id) ) {
        /* Update the pending faults table */
        p_fault = &g_fault_counters[fault_id];
        p_fault->ts_data[p_fault->head] = ts_data;
        ++p_fault->count;
        ++p_fault->head;
        /* Wrap the head so we're always writing fresh data over old data */
        if (FAI_N_TS_DATA == p_fault->head) {
            p_fault->head = 0;
        }
        g_pending_faults = TRUE;
    } else {
        /* Invalid fault ID */
    }
}


void task_fai(void)
{
    if (TRUE == g_pending_faults) {
        fai_write_faults_to_nvm();
        g_pending_faults = FALSE;
    }
}


static void fai_write_faults_to_nvm(void)
{
    SIZE_T i;
    SIZE_T len;
    U8_T address;

    address = FAI_NVM_BASE_ADDRESS;
    len = sizeof(FAI_FAULT_COUNTER_T);

    for (i = (SIZE_T) FAI_FAULT_ID_FIRST + 1u; (SIZE_T) FAI_FAULT_ID_N > i; ++i) {
        eeprom_write(address, &g_fault_counters[i], len);
        address += len;
    }
}


static void fai_read_faults_from_nvm(void)
{
    SIZE_T i;
    SIZE_T len;
    U8_T address;
    U8_T buf[sizeof(FAI_FAULT_COUNTER_T)];
    FAI_FAULT_COUNTER_T* p_new_fault;
    U8_T* p_existing_fault_bytes;

    address = FAI_NVM_BASE_ADDRESS;
    len = sizeof(FAI_FAULT_COUNTER_T);

    for (i = (SIZE_T) FAI_FAULT_ID_FIRST + 1u; (SIZE_T) FAI_FAULT_ID_N > i; ++i) {
        eeprom_read(address, buf, len);
        p_existing_fault_bytes = (U8_T*)(void*)&g_fault_counters[i];
        memcpy_by_U8(p_existing_fault_bytes, buf, len);
        address += len;
    }
}