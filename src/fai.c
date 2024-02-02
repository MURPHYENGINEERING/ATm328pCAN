#include "fai.h"
#include "eeprom.h"
#include "memory.h"

#define FAI_MAX_FAULTS U16_T_MAX

FAI_FAULT_COUNTER_T g_fault_counters[(SIZE_T) FAI_FAULT_ID_N];
BOOL_T g_pending_faults;

FAI_FAULT_COUNTER_T g_fai_fault_empty;


static void fai_write_faults_to_nvm(void);


/*******************************************************************************
 * Erase the fault section of the NVM and set all fault counters to 0.
 ******************************************************************************/
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

    g_pending_faults = FALSE;
}


/*******************************************************************************
 * Initialize the fault counters to zero.
 ******************************************************************************/
void fai_init(void)
{
    BOOL_T init_flag;

    memset_by_U8((U8_T*)(void*)&g_fai_fault_empty, 0, sizeof(g_fai_fault_empty));

    init_flag = (BOOL_T) eeprom_read_byte(FAI_NVM_INIT_FLAG_ADDRESS);
    if (FALSE == init_flag) {
        fai_clear_faults();
        init_flag = TRUE;
        eeprom_write_byte(FAI_NVM_INIT_FLAG_ADDRESS, init_flag);
    }

    fai_read_faults_from_nvm();

    g_pending_faults = FALSE;
}


/*******************************************************************************
 * Log a fault, `PASS` or `FAIL`, with the given troubleshooting data.
 * \param[in] fault_id  The ID of the fault to be logged.
 * \param[in] indicator Whether the fault is `PASS` or `FAIL`.
 * \param[in] ts_data   The fault troubleshooting data to be logged.
 ******************************************************************************/
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
            for (i = 0; FAI_TS_DATA_LEN > i; ++i) {
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
            if (FAI_TS_DATA_LEN == p_fault->head) {
                p_fault->head = (U8_T) 0;
            }
        }
        g_pending_faults = TRUE;
    } else {
        fai_pass_fail_logger(FAI_FAULT_ID_BAD_FAULT, FAIL, (U32_T) fault_id);
    }
}


/*******************************************************************************
 * Report on the status of the given fault ID.
 * \param[in] fault_id The fault ID to be reported.
 * \return  The fault counter for the given fault, indicating number of faults
 *          and associated troubleshooting data.
 ******************************************************************************/
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


/*******************************************************************************
 * Perform the FAI task, writing faults to NVM if any new faults are pending.
 ******************************************************************************/
void task_fai(void)
{
    if (TRUE == g_pending_faults) {
        fai_write_faults_to_nvm();
        g_pending_faults = FALSE;
    }
}


/*******************************************************************************
 * Write the fault array to the NVM.
 ******************************************************************************/
static void fai_write_faults_to_nvm(void)
{
    eeprom_write(
        FAI_NVM_BASE_ADDRESS, 
        (U8_T*)(void*)g_fault_counters, 
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );
}


/*******************************************************************************
 * Read the fault array from NVM, overwriting any current faults. 
 ******************************************************************************/
void fai_read_faults_from_nvm(void)
{
    eeprom_read(
        FAI_NVM_BASE_ADDRESS, 
        (U8_T*)(void*)g_fault_counters, 
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );
}