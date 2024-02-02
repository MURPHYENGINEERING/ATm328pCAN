#ifndef __ATM328P_DSC_H__
#define __ATM328P_DSC_H__

#include "types.h"

/** Specifies a port as an output. */
#define DDR_OUTPUT TRUE
/** Specifies a port as an input. */
#define DDR_INPUT FALSE

/** Enables the pull-up resistor on a port. */
#define PORT_PULLUP TRUE

/** Port B Data Direction Register */
extern volatile REGISTER_T DDRB;
/** Port B Data Register */
extern volatile REGISTER_T PORTB;

/** Port C Data Direction Register */
extern volatile REGISTER_T DDRC;
/** Port C Data Register */
extern volatile REGISTER_T PORTC;
/** Pin C Data Register */
extern volatile REGISTER_T PINC;

/** Port D Data Direction Register */
extern volatile REGISTER_T DDRD;
/** Port D Data Register */
extern volatile REGISTER_T PORTD;


/** Analog GPIO */
typedef union {
    struct {
        VBOOL_T A0      : 1;
        VBOOL_T A1      : 1;
        VBOOL_T A2      : 1;
        VBOOL_T A3      : 1;
        VBOOL_T A4      : 1;
        VBOOL_T A5      : 1;
        VBOOL_T RESET   : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} DSC_ANALOG_T;

/** Analog Data Direction Register */
extern volatile DSC_ANALOG_T DDR_ANALOG;
/** Analog Data Register */
extern volatile DSC_ANALOG_T PORT_ANALOG;
/** Analog Data Input Register */
extern volatile DSC_ANALOG_T PIN_ANALOG;


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
} DSC_BUILTIN_T;

/** Data Direction Register for Arduino UNO built-in external peripherals. */
extern volatile DSC_BUILTIN_T DDR_BUILTIN;
/** Port Register for Arduino UNO built-in external peripherals. */
extern volatile DSC_BUILTIN_T PORT_BUILTIN;


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
        VBOOL_T LED1    : 1;
        /** The MCP2515 shield built-in LED 2 */
        VBOOL_T LED2    : 1;
    } bits;
    VU8_T byte;
} DSC_CANBOARD_T;

/** Data Direction Register for MCP2515 shield built-in external 
  * peripherals. */
extern volatile DSC_CANBOARD_T DDR_CANBOARD;
/** Port Register for MCP2515 shield built-in external peripherals. */
extern volatile DSC_CANBOARD_T PORT_CANBOARD;




#endif