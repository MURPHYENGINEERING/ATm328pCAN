#include "fai.h"
#include "eeprom.h"
#include "memory.h"

#define FAI_MAX_FAULTS U16_T_MAX

FAI_FAULT_COUNTER_T g_fault_counters[(SIZE_T) FAI_FAULT_ID_N];
BOOL_T g_pending_faults;

FAI_FAULT_COUNTER_T g_fai_fault_empty;


static void fai_write_faults_to_nvm(void);
static void fai_read_faults_from_nvm(void);


void fai_clear_faults(void)
{
    eeprom_erase(
        FAI_NVM_BASE_ADDRESS,
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );

    memset_by_U8(
        (U8_T*)(void*)g_fault_counters, 
        (U8_T) 0, 
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );
}


void fai_init(void)
{
    memset_by_U8((U8_T*)(void*)&g_fai_fault_empty, 0, sizeof(g_fai_fault_empty));

    memset_by_U8(
        (U8_T*)(void*)g_fault_counters, 
        (U8_T) 0, 
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );

    g_pending_faults = FALSE;
}


void fai_pass_fail_logger(
    FAI_FAULT_ID_T fault_id, 
    PASS_T indicator, 
    U32_T ts_data
)
{
    SIZE_T i;
    FAI_FAULT_COUNTER_T* p_fault;

    if ( FAI_FAULT_ID_N > fault_id ) {
        /* Update the pending faults table */
        p_fault = &g_fault_counters[fault_id];
        if (PASS == indicator) {
            p_fault->count = (U8_T) 0;
            p_fault->head = (U8_T) 0;
            for (i = 0; FAI_TS_DATA_N > i; ++i) {
                p_fault->ts_data[i] = (U32_T) 0;
            }
        } else {
            /* Add a fault to the counter for the given ID */
            p_fault->ts_data[p_fault->head] = ts_data;
            ++p_fault->count;
            ++p_fault->head;
            /* Wrap the head so we're always writing fresh data over old data */
            /* Don't wrap the fault counter so we can count them even if they
             * aren't stored. */
            if (FAI_TS_DATA_N == p_fault->head) {
                p_fault->head = (U8_T) 0;
            }
        }
        g_pending_faults = TRUE;
    } else {
        fai_pass_fail_logger(FAI_FAULT_ID_BAD_FAULT, FAIL, (U32_T) fault_id);
    }
}


FAI_FAULT_COUNTER_T fai_fault_reporter(FAI_FAULT_ID_T fault_id)
{
    FAI_FAULT_COUNTER_T fault;

    if ( FAI_FAULT_ID_N > fault_id ) {
        fault = g_fault_counters[(SIZE_T) fault_id];
    } else {
        fai_pass_fail_logger(FAI_FAULT_ID_BAD_FAULT, FAIL, (U32_T) fault_id);
        fault = g_fai_fault_empty;
    }

    return fault;
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

    for (i = (SIZE_T) 0; (SIZE_T) FAI_FAULT_ID_N > i; ++i) {
        eeprom_write(address, (U8_T*)(void*)&g_fault_counters[i], len);
        address += len;
    }
}


static void fai_read_faults_from_nvm(void)
{
    SIZE_T i;
    SIZE_T len;
    U8_T address;
    U8_T buf[sizeof(FAI_FAULT_COUNTER_T)];
    U8_T* p_existing_fault_bytes;

    address = FAI_NVM_BASE_ADDRESS;
    len = sizeof(FAI_FAULT_COUNTER_T);

    for (i = (SIZE_T) 0; (SIZE_T) FAI_FAULT_ID_N > i; ++i) {
        eeprom_read(address, buf, len);
        p_existing_fault_bytes = (U8_T*)(void*)&g_fault_counters[i];
        memcpy_by_U8(p_existing_fault_bytes, buf, len);
        address += len;
    }
}