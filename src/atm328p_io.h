#ifndef __IO_H__
#define __IO_H__

#include "types.h"

#define DDR_OUTPUT 1
#define DDR_INPUT 0

/* Port B Data Direction Register */
extern volatile REGISTER_T DDRB;
/* Port B Data Register */
extern volatile REGISTER_T PORTB;


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




#endif