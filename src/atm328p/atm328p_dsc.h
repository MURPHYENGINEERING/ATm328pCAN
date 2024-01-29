#ifndef __ATM328P_DSC_H__
#define __ATM328P_DSC_H__

#include "types.h"

#define DDR_OUTPUT 1u
#define DDR_INPUT 0

#define PORT_PULLUP 1u

/* Port B Data Direction Register */
extern volatile REGISTER_T DDRB;
/* Port B Data Register */
extern volatile REGISTER_T PORTB;

/* Port D Data Direction Register */
extern volatile REGISTER_T DDRD;
/* Port D Data Register */
extern volatile REGISTER_T PORTD;


typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T LED : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} BUILTIN_T;

extern volatile BUILTIN_T DDR_BUILTIN;
extern volatile BUILTIN_T PORT_BUILTIN;


typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        VBOOL_T LED1 : 1;
        VBOOL_T LED2 : 1;
    } bits;
    VU8_T byte;
} CANBOARD_T;

extern volatile CANBOARD_T DDR_CANBOARD;
extern volatile CANBOARD_T PORT_CANBOARD;




#endif