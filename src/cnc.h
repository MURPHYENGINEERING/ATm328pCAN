#ifndef __CNC_H__
#define __CNC_H__

/** List of possible commands that can be sent to Command and Control. */
typedef enum {
    /** Command the FAI to clear faults and erase NVM. */
    CNC_CMD_CLEAR_FAULTS,
    /** Command the FAI to report on all active faults. */
    CNC_CMD_REPORT_FAULTS,
    /** Command the CAN driver to send a message whose identifier and data will
      * follow this command. */
    CNC_CMD_SEND_CAN_MSG,
    /** Report the checksum of the ROM BIT */
    CNC_CMD_BITROM_CHECKSUM,
    /** Number of Command and Control commands. */
    CNC_CMD_N
} CNC_CMD_T;


void cnc_init(void);

void task_cnc_rx(void);

#endif