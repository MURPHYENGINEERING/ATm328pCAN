#ifndef __TYPES_H__
#define __TYPES_H__

typedef _Bool BOOL_T;
typedef volatile BOOL_T VBOOL_T;

typedef char U8_T;
typedef volatile U8_T VU8_T;

typedef short U16_T;
typedef volatile U16_T VU16_T;

typedef unsigned int U32_T;
typedef volatile U32_T VU32_T;
typedef int S32_T;


typedef union {
    struct {
        VBOOL_T bit0 : 1;
        VBOOL_T bit1 : 1;
        VBOOL_T bit2 : 1;
        VBOOL_T bit3 : 1;
        VBOOL_T bit4 : 1;
        VBOOL_T bit5 : 1;
        VBOOL_T bit6 : 1;
        VBOOL_T bit7 : 1;
    } bits;
    VU8_T byte;
} REGISTER_T;


typedef union {
    struct {
        volatile REGISTER_T low;
        volatile REGISTER_T high;
    } bytes;
    VU8_T data[2];
    VU16_T halfword;
} HALFWORD_T;


typedef union {
    struct {
        volatile REGISTER_T byte0;
        volatile REGISTER_T byte1;
        volatile REGISTER_T byte2;
        volatile REGISTER_T byte3;
    } bytes;
    struct {
        VU16_T low;
        VU16_T high;
    } halfwords;
    VU8_T data[4];
    VU32_T word;
} WORD_T;


#define TRUE 1
#define FALSE 0

#define PASS 1
#define FAIL 0

#define GND 1
#define OPEN 0

#define NULL 0


#endif