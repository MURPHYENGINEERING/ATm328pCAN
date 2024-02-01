#include "cnc.h"
#include "usart.h"
#include "fai.h"
#include "can.h"
#include "memory.h"
#include "fifo.h"
#include "serialize.h"


/** Maximum number of commands in the RX FIFO queue. */
#define CNC_RX_FIFO_LEN 10
/** Command and Control RX FIFO queue underlying buffer. */
FIFO_ENTRY_T g_cnc_rx_q_buf[CNC_RX_FIFO_LEN];
/** Command and Control RX FIFO queue. */
FIFO_T g_cnc_rx_q;

/** Maximum length in bytes of the Command and Control RX buffer. */
#define CNC_RX_BUF_LEN 20
/** Command and Control RX buffer. */
U8_T g_cnc_rx_buf[CNC_RX_BUF_LEN];
/** Write index into the Command and Control RX buffer. */
SIZE_T g_cnc_rx_buf_write_idx;

/** Strings representing the commands in `CNC_CMT_T`. These are matched against
  * commands coming in on the USART. */
U8_T* g_cnc_cmd_strings[(SIZE_T) CNC_CMD_N] = {
    (U8_T*) "clearf",
    (U8_T*) "reportf",
    (U8_T*) "cansend"
};

static void cnc_process_q(void);
static void cnc_process_cmd(U8_T* buf, SIZE_T len);
static void cnc_cmd_clear_faults(void);
static void cnc_cmd_report_faults(void);
static void cnc_cmd_send_can_msg(void);

/*******************************************************************************
 * Initialize the Command and Control buffers and prepare to receive commands via
 * USART.
 ******************************************************************************/
void cnc_init(void)
{
    fifo_q_init(&g_cnc_rx_q, g_cnc_rx_q_buf, CNC_RX_FIFO_LEN);
}


/*******************************************************************************
 * Perform the Command and Control Receive Data task.
 * This task receives data from the USART device and processes commands it sees.
 ******************************************************************************/
void task_cnc_rx(void)
{
    SIZE_T len;
    SIZE_T i;
    FIFO_STATUS_T status;

    /* Read up to CNC_RX_BUF_LEN bytes from USART into the CNC RX buffer. */
    len = usart_rx(
        &g_cnc_rx_buf[g_cnc_rx_buf_write_idx], 
        CNC_RX_BUF_LEN - g_cnc_rx_buf_write_idx
    );

    g_cnc_rx_buf_write_idx += len;

    /* Look for a command terminator */
    for (i = 0; i < g_cnc_rx_buf_write_idx; ++i) {
        if ((U8_T) '\n' == g_cnc_rx_buf[i]) {
            /* Don't process a command if the buffer is actually empty. */
            if (0 < i) {
                /* Omit the newline */
                status = fifo_q_add(&g_cnc_rx_q, g_cnc_rx_buf, i-1);
                if (FIFO_FULL == status) {
                    fai_pass_fail_logger(
                        FAI_FAULT_ID_CNC_RX_Q_OVERFLOW, 
                        FAIL, 
                        (U32_T) 0
                    );
                }
            }
            /* Clear the buffer */
            g_cnc_rx_buf_write_idx = (SIZE_T) 0;
        }
    }

    cnc_process_q();
}

/*******************************************************************************
 * Process the incoming command FIFO queue, removing commands and command 
 * arguments and processing them one by one.
 ******************************************************************************/
static void cnc_process_q(void)
{
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T len;

    do {
        status = fifo_q_remove(&g_cnc_rx_q, buf, &len);

        if (FIFO_OK == status) {
            cnc_process_cmd(buf, len);
        }
        
    } while (FIFO_OK == status);
}

/*******************************************************************************
 * Process a command in the given buffer by comparing it to the list of command
 * strings.
 * \param[in] buf   The buffer containing the command text to be compared.
 * \param[in] len   Number of bytes in the command text buffer.
 ******************************************************************************/
static void cnc_process_cmd(U8_T* buf, SIZE_T len)
{
    if (0 == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_CLEAR_FAULTS], len)) {
        cnc_cmd_clear_faults();
    } else if (0 == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_REPORT_FAULTS], len)) {
        cnc_cmd_report_faults();
    } else if (0 == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_SEND_CAN_MSG], len)) {
        cnc_cmd_send_can_msg();
    }
}


/*******************************************************************************
 * Perform the "clear faults" command by calling on FAI to erase NVM.
 ******************************************************************************/
static void cnc_cmd_clear_faults(void)
{
    fai_clear_faults();
    usart_tx("Faults cleared\n", 5u);
}

/*******************************************************************************
 * Perform the "report faults" command by retrieving faults from FAI and writing
 * them to the USART device.
 ******************************************************************************/
static void cnc_cmd_report_faults(void)
{
    FAI_FAULT_ID_T fault_id;
    FAI_FAULT_COUNTER_T fault;
    /* + 1 for newline */
    U8_T buf[sizeof(FAI_FAULT_ID_T) + sizeof(FAI_FAULT_COUNTER_T) + 1u];
    U8_T* p_buf;

    fault_id = FAI_FAULT_ID_FIRST;

    while (fault_id != FAI_FAULT_ID_N) {
        fault = fai_fault_reporter(fault_id);
        if (0 < fault.count) {
            p_buf = serialize_fault(buf, fault_id, &fault);
            *p_buf = '\n';
            ++p_buf;
            usart_tx(buf, p_buf - buf);
        }

        ++fault_id;
    }

    usart_tx((U8_T*) "done\n", 5u);
}

/*******************************************************************************
 * Perform the "send CAN message" command by retrieving the message parameters
 * from the CNC FIFO queue and writing them to the CAN device.
 ******************************************************************************/
static void cnc_cmd_send_can_msg(void)
{
}