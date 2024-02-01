#ifndef __ATM328P_DSC_H__
#define __ATM328P_DSC_H__

#include "types.h"

#define DDR_OUTPUT TRUE
#define DDR_INPUT FALSE

#define PORT_PULLUP TRUE

/** Port B Data Direction Register */
extern volatile REGISTER_T DDRB;
/** Port B Data Register */
extern volatile REGISTER_T PORTB;

/** Port D Data Direction Register */
extern volatile REGISTER_T DDRD;
/** Port D Data Register */
extern volatile REGISTER_T PORTD;


/** External peripherals built into the Arduino UNO development board. */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        /** The Arduino UNO built-in LED */
        VBOOL_T LED : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} BUILTIN_T;

/** Data Direction Register for Arduino UNO built-in external peripherals. */
extern volatile BUILTIN_T DDR_BUILTIN;
/** Port Register for Arduino UNO built-in external peripherals. */
extern volatile BUILTIN_T PORT_BUILTIN;


/** External peripherals built into the MCP2515 shield. */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        /** The MCP2515 shield built-in LED 1 */
        VBOOL_T LED1 : 1;
        /** The MCP2515 shield built-in LED 2 */
        VBOOL_T LED2 : 1;
    } bits;
    VU8_T byte;
} CANBOARD_T;

/** Data Direction Register for MCP2515 shield built-in external 
  * peripherals. */
extern volatile CANBOARD_T DDR_CANBOARD;
/** Port Register for MCP2515 shield built-in external peripherals. */
extern volatile CANBOARD_T PORT_CANBOARD;




#endif