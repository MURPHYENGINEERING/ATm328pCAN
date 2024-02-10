#include "fai.h"
#include "eeprom.h"
#include "memory.h"

#define FAI_MAX_FAULTS U8_T_MAX

/** Located at the address of the "faults have been initialized" flag in NVM. */
extern volatile U8_T __ld_fai_flag;
/** Located at the base address in the NVM where fault data is stored. */
extern volatile U8_T __ld_fai_base;


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
        (SIZE_T) &__ld_fai_base,
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );

    memset(
        g_fault_counters, 
        (U8_T) 0u, 
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

    memset(&g_fai_fault_empty, 0, sizeof(g_fai_fault_empty));

    init_flag = (BOOL_T) eeprom_read_byte((SIZE_T) &__ld_fai_flag);
    if (TRUE == init_flag) {
        fai_clear_faults();
        init_flag = FALSE;
        eeprom_write_byte((SIZE_T) &__ld_fai_flag, init_flag);
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
            p_fault->count = (U8_T) 0u;
            p_fault->head = (U8_T) 0u;
            for (i = 0; FAI_TS_DATA_LEN > i; ++i) {
                p_fault->ts_data[i] = (U32_T) 0u;
            }
        } else {
            /* Add a fault to the counter for the given ID */
            p_fault->ts_data[p_fault->head] = ts_data;
            if (p_fault->count < FAI_MAX_FAULTS) {
                ++p_fault->count;
            }
            p_fault->head = (U16_T)( (p_fault->head + 1) % FAI_TS_DATA_LEN );
        }
        /* We can't trigger a write on task overrun because writing inherently
         * causes a task overrun :( */
        if (FAI_FAULT_ID_TASK_OVERRUN != fault_id) {
            g_pending_faults = TRUE;
        }
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
        (SIZE_T) &__ld_fai_base, 
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
        (SIZE_T) &__ld_fai_base, 
        (U8_T*)(void*) g_fault_counters, 
        sizeof(FAI_FAULT_COUNTER_T) * (SIZE_T) FAI_FAULT_ID_N
    );
}