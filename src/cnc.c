#include "cnc.h"
#include "usart.h"
#include "fai.h"
#include "can.h"
#include "memory.h"
#include "fifo.h"
#include "serialize.h"
#include "string.h"
#include "bit.h"


/** Maximum number of commands in the RX FIFO queue. */
#define CNC_RX_FIFO_LEN 10u
/** Command and Control RX FIFO queue underlying buffer. */
FIFO_ENTRY_T g_cnc_rx_q_buf[CNC_RX_FIFO_LEN];
/** Command and Control RX FIFO queue. */
FIFO_T g_cnc_rx_q;

/** Maximum number of data words for any command. */
#define CNC_RX_DATA_FIFO_LEN 3u
/** Command data RX FIFO queue underlying buffer. */
FIFO_ENTRY_T g_cnc_data_q_buf[CNC_RX_DATA_FIFO_LEN];
/** Command data RX FIFO queue. */
FIFO_T g_cnc_data_q;

/** Maximum length in bytes of the Command and Control RX buffer. */
#define CNC_RX_BUF_LEN 20u
/** Command and Control RX buffer. */
U8_T g_cnc_rx_buf[CNC_RX_BUF_LEN];
/** Write index into the Command and Control RX buffer. */
SIZE_T g_cnc_rx_buf_write_idx;

/** Strings representing the commands in `CNC_CMT_T`. These are matched against
  * commands coming in on the USART. */
U8_T* g_cnc_cmd_strings[(SIZE_T) CNC_CMD_N] = {
    (U8_T*) "fclear",
    (U8_T*) "freport",
    (U8_T*) "cansend",
    (U8_T*) "bitromcs"
};

static void cnc_process_q(void);
static void cnc_process_cmd(U8_T* buf, SIZE_T len);
static void cnc_cmd_clear_faults(void);
static void cnc_cmd_report_faults(void);
static void cnc_cmd_send_can_msg(void);
static void cnc_cmd_get_bit_rom_checksum(void);

/*******************************************************************************
 * Initialize the Command and Control buffers and prepare to receive commands via
 * USART.
 ******************************************************************************/
void cnc_init(void)
{
    fifo_q_init(&g_cnc_rx_q, g_cnc_rx_q_buf, CNC_RX_FIFO_LEN);
    fifo_q_init(&g_cnc_data_q, g_cnc_data_q_buf, CNC_RX_DATA_FIFO_LEN);
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
            if (0u < i) {
                /* Omit the newline */
                status = fifo_q_add(&g_cnc_rx_q, g_cnc_rx_buf, i-1);
                if (FIFO_FULL == status) {
                    fai_pass_fail_logger(
                        FAI_FAULT_ID_CNC_RX_Q_OVERFLOW, 
                        FAIL, 
                        (U32_T) 0u
                    );
                }
            }
            /* Clear the buffer */
            g_cnc_rx_buf_write_idx = (SIZE_T) 0u;
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
    if (0u == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_CLEAR_FAULTS], len)) {
        cnc_cmd_clear_faults();
    } else if (0u == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_REPORT_FAULTS], len)) {
        cnc_cmd_report_faults();
    } else if (0u == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_SEND_CAN_MSG], len)) {
        cnc_cmd_send_can_msg();
    } else if (0u == memcmp_by_U8(buf, g_cnc_cmd_strings[(SIZE_T) CNC_CMD_BITROM_CHECKSUM], len)) {
        cnc_cmd_get_bit_rom_checksum();
    } else {
        /** Command data, put it on the data queue for processing when the command
          * comes through. */
        fifo_q_add(&g_cnc_data_q, buf, len);
    }
}


/*******************************************************************************
 * Perform the "clear faults" command by calling on FAI to erase NVM.
 ******************************************************************************/
static void cnc_cmd_clear_faults(void)
{
    fai_clear_faults();
    usart_tx((U8_T*) "Faults cleared\n", 15u);
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
    FIFO_STATUS_T status;
    U8_T buf[FIFO_DATA_LEN];
    SIZE_T len;

    CAN_IDENT_T identifier;

    status = fifo_q_remove(&g_cnc_data_q, buf, &len);
    if (status == FIFO_OK) {
        if (2 <= len) {
            identifier = deserialize_can_identifier(buf);

            status = fifo_q_remove(&g_cnc_data_q, buf, &len);
            if (status == FIFO_OK) {
                can_tx_q_add(identifier, buf, len);
            } else {
                fai_pass_fail_logger(FAI_FAULT_ID_CNC_SENDCAN_MALFORMED, FAIL, (U32_T) 2u);
            }
        } else {
            fai_pass_fail_logger(FAI_FAULT_ID_CNC_SENDCAN_MALFORMED, FAIL, (U32_T) 1u);
        }
    } else {
        fai_pass_fail_logger(FAI_FAULT_ID_CNC_SENDCAN_MALFORMED, FAIL, (U32_T) 0u);
    }
}


/*******************************************************************************
 * Retrieve the ROM BIT checksum and output it on USART.
 ******************************************************************************/
void cnc_cmd_get_bit_rom_checksum(void)
{
    U32_T checksum;
    U8_T buf[11];
    SIZE_T len;
    SIZE_T i;

    checksum = bit_rom_get_checksum();

    len = itoa(buf, checksum);
    i = (SIZE_T) 0u;

    buf[len] = '\n';
    ++len;

    while (i < len) {
        i += usart_tx(&buf[i], len - i);
    }
}


USART_CFG_T cnc_get_usart_cfg(void)
{
    /* Configure USART to talk to the CNC laptop via USB. */
    USART_CFG_T cfg;

    cfg.mode             = USART_MODE_ASYNCHRONOUS;
    cfg.baud             = USART_BAUD_9600;
    cfg.character_size   = USART_CHARACTER_SIZE_8;
    cfg.stop_bits        = USART_STOP_BITS_1;
    cfg.parity           = USART_PARITY_MODE_EVEN;

    return cfg;
}